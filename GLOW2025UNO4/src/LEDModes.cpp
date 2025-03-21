#include "LEDModes.h"

const int LEDModes::colorCycle[][3] = {
    // Red to Green
    {255, 0, 0}, {247, 8, 0}, {238, 17, 0}, {230, 25, 0}, {221, 34, 0}, {213, 42, 0}, {204, 51, 0},
    {196, 59, 0}, {187, 68, 0}, {179, 76, 0}, {170, 85, 0}, {162, 93, 0}, {153, 102, 0}, {145, 110, 0},
    {136, 119, 0}, {128, 127, 0}, {119, 136, 0}, {110, 145, 0}, {102, 153, 0}, {93, 162, 0}, {85, 170, 0},
    {76, 179, 0}, {68, 187, 0}, {59, 196, 0}, {51, 204, 0}, {42, 213, 0}, {34, 221, 0}, {25, 230, 0},
    {17, 238, 0}, {8, 247, 0}, {0, 255, 0},

    // Green to Blue
    {0, 255, 0}, {0, 247, 8}, {0, 238, 17}, {0, 230, 25}, {0, 221, 34}, {0, 213, 42}, {0, 204, 51},
    {0, 196, 59}, {0, 187, 68}, {0, 179, 76}, {0, 170, 85}, {0, 162, 93}, {0, 153, 102}, {0, 145, 110},
    {0, 136, 119}, {0, 128, 127}, {0, 119, 136}, {0, 110, 145}, {0, 102, 153}, {0, 93, 162}, {0, 85, 170},
    {0, 76, 179}, {0, 68, 187}, {0, 59, 196}, {0, 51, 204}, {0, 42, 213}, {0, 34, 221}, {0, 25, 230},
    {0, 17, 238}, {0, 8, 247}, {0, 0, 255},

    // Blue to Red
    {0, 0, 255}, {8, 0, 247}, {17, 0, 238}, {25, 0, 230}, {34, 0, 221}, {42, 0, 213}, {51, 0, 204},
    {59, 0, 196}, {68, 0, 187}, {76, 0, 179}, {85, 0, 170}, {93, 0, 162}, {102, 0, 153}, {110, 0, 145},
    {119, 0, 136}, {127, 0, 128}, {136, 0, 119}, {145, 0, 110}, {153, 0, 102}, {162, 0, 93}, {170, 0, 85},
    {179, 0, 76}, {187, 0, 68}, {196, 0, 59}, {204, 0, 51}, {213, 0, 42}, {221, 0, 34}, {230, 0, 25},
    {238, 0, 17}, {247, 0, 8}, {255, 0, 0}
};

const int LEDModes::numColors = sizeof(LEDModes::colorCycle) / sizeof(LEDModes::colorCycle[0]);

LEDModes::LEDModes(int pin, int numLEDs) 
    : NUM_LEDS(numLEDs),
      currentIndex(0), 
      currentHighlight_1(0), 
      currentHighlight_2(numLEDs/2), 
      colorDelayTime(100),
      highlightDelayTime(100),
      targetColorDelay(100),
      targetHighlightDelay(100),
      lastColorUpdate(0),
      lastHighlightUpdate(0),
      transitionStartTime(0),
      startColorDelay(0),
      startHighlightDelay(0),
      warningStartTime(0),
      lastInputTime(0),
      warningPos(0),
      isInWarningState(false) {
    leds = new CRGB[numLEDs];
    FastLED.addLeds<WS2812B, 3, GRB>(leds, numLEDs);  // Assuming WS2812B LEDs on pin 3
}

void LEDModes::begin() {
    FastLED.setBrightness(255);
    FastLED.show();
}

void LEDModes::setDelay(String ppl_L_R) {
    // Check if enough time has passed since last input
    if (millis() - lastInputTime < WARNING_DURATION) {
        return;  // Ignore input during cooldown
    }

    int colonIndex = ppl_L_R.indexOf(':');  

    if (colonIndex != -1) {
        String leftStr = ppl_L_R.substring(0, colonIndex);
        String rightStr = ppl_L_R.substring(colonIndex + 1);
        
        if (leftStr.length() > 0 && rightStr.length() > 0 && 
            isDigit(leftStr[0]) && isDigit(rightStr[0])) {
            
            int ppl_L = leftStr.toInt();
            int ppl_R = rightStr.toInt();
            
            ppl_T = ppl_L + ppl_R;
            ppl_D = ppl_L - ppl_R;

            if (ppl_T > 5) {
                // Start warning animation
                warningStartTime = millis();
                lastInputTime = millis();
                isInWarningState = true;
                warningPos = 0;
                return;
            }
            
            isInWarningState = false;
            
            // Store current delays as starting points
            startColorDelay = colorDelayTime;
            startHighlightDelay = highlightDelayTime;
            
            // Calculate target delays using exponential curve
            // Using sqrt and normalizing to 5 people instead of 10
            float totalFactor = sqrt(ppl_T) / sqrt(5.0);  // normalized to 5 people
            float diffFactor = sqrt(abs(ppl_D)) / sqrt(5.0);
            
            // Adjust range to go from 100ms to 10ms with 5 people
            targetColorDelay = 100 - (90 * totalFactor);  // 90 = difference between 100 and 10
            targetHighlightDelay = 100 - (90 * diffFactor);
            
            // Ensure minimum delay of 5ms
            if (targetColorDelay < 5) targetColorDelay = 5;
            if (targetHighlightDelay < 5) targetHighlightDelay = 5;
            
            // Start the transition
            transitionStartTime = millis();
        }
        return;
    }
}

void LEDModes::updateIntensity() {  
    if (isInWarningState || ppl_T > 5) {
        static unsigned long lastUpdate = 0;
        static const int UPDATE_INTERVAL = 10; // 10ms between updates
        
        if (millis() - lastUpdate >= UPDATE_INTERVAL) {
            // Update each LED with a random color from the colorCycle array
            for (int i = 0; i < NUM_LEDS; i++) {
                // Pick a random color from the colorCycle array
                int randomColorIndex = random(0, numColors);
                leds[i] = CRGB(
                    colorCycle[randomColorIndex][0],
                    colorCycle[randomColorIndex][1],
                    colorCycle[randomColorIndex][2]
                );
            }
            
            FastLED.show();
            lastUpdate = millis();
        }

        // Check if warning duration is over (only for initial warning)
        if (isInWarningState && millis() - warningStartTime >= WARNING_DURATION) {
            isInWarningState = false;
        }
        return;
    }

    // Update delays smoothly if in transition
    if (millis() - transitionStartTime < TRANSITION_DURATION) {
        float progress = (float)(millis() - transitionStartTime) / TRANSITION_DURATION;
        progress = min(1.0f, progress); // Ensure we don't exceed 1.0
        
        // Smoother easing function (cubic ease-in-out)
        progress = progress < 0.5
            ? 4 * progress * progress * progress
            : 1 - pow(-2 * progress + 2, 3) / 2;
        
        // Interpolate between start and target delays
        colorDelayTime = startColorDelay + (targetColorDelay - startColorDelay) * progress;
        highlightDelayTime = startHighlightDelay + (targetHighlightDelay - startHighlightDelay) * progress;
    }

    bool shouldUpdateColor = (millis() - lastColorUpdate >= colorDelayTime);
    bool shouldUpdateHighlight = (millis() - lastHighlightUpdate >= highlightDelayTime);
    
    if (!shouldUpdateColor && !shouldUpdateHighlight) return;

    if (ppl_T > 5) {
        fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
        FastLED.show();
        return;
    }

    // First turn all LEDs off
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));

    // Only update LEDs near the highlights
    for (int highlight : {currentHighlight_1, currentHighlight_2}) {
        for (int offset = -4; offset <= 4; offset++) {
            int i = (highlight + offset + NUM_LEDS) % NUM_LEDS;  // Ensure positive wraparound
            float intensity = 1.0 - (0.8 * (abs(offset) / 4.0));

            leds[i] = CRGB(
                colorCycle[currentIndex][0] * intensity,
                colorCycle[currentIndex][1] * intensity,
                colorCycle[currentIndex][2] * intensity
            );
        }
    }

    FastLED.show();

    if (shouldUpdateColor) {
        currentIndex = (currentIndex + 1) % numColors;
        lastColorUpdate = millis();
    }

    if (shouldUpdateHighlight) {
        if (ppl_D > 0) {
            currentHighlight_1 = (currentHighlight_1 + 1) % NUM_LEDS;
            currentHighlight_2 = (currentHighlight_2 + 1) % NUM_LEDS;
        } else if (ppl_D < 0) {
            currentHighlight_1 = (currentHighlight_1 - 1 + NUM_LEDS) % NUM_LEDS;
            currentHighlight_2 = (currentHighlight_2 - 1 + NUM_LEDS) % NUM_LEDS;
        }
        lastHighlightUpdate = millis();
    }
}

void LEDModes::updatePulse() {
    bool shouldUpdateHighlight = (millis() - lastHighlightUpdate >= highlightDelayTime);
    
    if (!shouldUpdateHighlight) return;

    for (int i = 0; i < NUM_LEDS; i++) {
        float intensity = 0.2;

        if (currentHighlight_1 - 4 <= i && i < currentHighlight_1 + 4 || 
            currentHighlight_2 - 4 <= i && i < currentHighlight_2 + 4) {
            intensity = 1.0 - (0.8 * (abs(i - currentHighlight_1) / 4.0));
        }

        leds[i] = CRGB(
            colorCycle[0][0] * intensity,
            colorCycle[0][1] * intensity,
            colorCycle[0][2] * intensity
        );
    }

    FastLED.show();

    if (ppl_D > 0) {
        currentHighlight_1 = (currentHighlight_1 + 1) % NUM_LEDS;
        currentHighlight_2 = (currentHighlight_2 + 1) % NUM_LEDS;
    } else if (ppl_D < 0) {
        currentHighlight_1 = (currentHighlight_1 - 1 + NUM_LEDS) % NUM_LEDS;
        currentHighlight_2 = (currentHighlight_2 - 1 + NUM_LEDS) % NUM_LEDS;
    }

    lastHighlightUpdate = millis();
}

int LEDModes::getColorDelay() {
    return colorDelayTime;
}

int LEDModes::getHighlightDelay() {
    return highlightDelayTime;
}

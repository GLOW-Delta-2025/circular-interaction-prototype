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
    : strip(numLEDs, pin, NEO_GRB + NEO_KHZ800), 
      currentIndex(0), 
      currentHighlight_1(0), 
      currentHighlight_2(numLEDs/2), 
      colorDelayTime(100),
      highlightDelayTime(100),
      lastColorUpdate(0),
      lastHighlightUpdate(0) {}

void LEDModes::begin() {
    strip.begin();
    strip.show();
}

void LEDModes::setDelay(String ppl_L_R) {
    int colonIndex = ppl_L_R.indexOf(':');  

    if (colonIndex != -1) {
        // Validate string format
        String leftStr = ppl_L_R.substring(0, colonIndex);
        String rightStr = ppl_L_R.substring(colonIndex + 1);
        
        // Check if both parts are valid numbers
        if (leftStr.length() > 0 && rightStr.length() > 0 && 
            isDigit(leftStr[0]) && isDigit(rightStr[0])) {
            
            int ppl_L = leftStr.toInt();
            int ppl_R = rightStr.toInt();
            
            ppl_T = ppl_L + ppl_R;
            ppl_D = ppl_L - ppl_R;
            
            colorDelayTime = 100 - (ppl_T * 10);
            highlightDelayTime = 100 - (abs(ppl_D) * 10);
            
            if (colorDelayTime < 5) colorDelayTime = 5;
            if (highlightDelayTime < 5) highlightDelayTime = 5;            
        }
        return;
    }
/*-------------------------------------- GREEN ERROR FEEDBACK ------------------------------------------------------------ */
/**/    for (int i = 0; i < strip.numPixels(); i++) {                                                                    /**/
/**/        strip.setPixelColor(i, strip.Color(0, 128, 0));                                                              /**/
/**/    }                                                                                                                /**/   
/**/    strip.show();                                                                                                    /**/ 
/**/    delay(500);                                                                                                      /**/               
/*-------------------------------------------------------------------------------------------------------------------------*/
}

void LEDModes::updateIntensity() {  
    bool shouldUpdateColor = (millis() - lastColorUpdate >= colorDelayTime);
    bool shouldUpdateHighlight = (millis() - lastHighlightUpdate >= highlightDelayTime);
    
    if (!shouldUpdateColor && !shouldUpdateHighlight) return;

    if (ppl_T > 10) {
        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(255, 0, 0));
        }
        strip.show();
        return;
    }

    // Update colors and highlights
    for (int i = 0; i < strip.numPixels(); i++) {
        float intensity = 0.2;
        for (int highlight : {currentHighlight_1, currentHighlight_2}) {
            int distance = abs(i - highlight);
            if (distance <= 4) { 
                intensity = 1.0 - (0.8 * (distance / 4.0));
                break;
            }
        }

        int r = colorCycle[currentIndex][0] * intensity;
        int g = colorCycle[currentIndex][1] * intensity;
        int b = colorCycle[currentIndex][2] * intensity;

        strip.setPixelColor(i, strip.Color(r, g, b));
    }

    strip.show();

    if (shouldUpdateColor) {
        currentIndex = (currentIndex + 1) % numColors;
        lastColorUpdate = millis();
    }

    if (shouldUpdateHighlight) {
        // Only move highlights if ppl_D is not 0
        if (ppl_D > 0) {
            // Move right
            currentHighlight_1 = (currentHighlight_1 + 1) % strip.numPixels();
            currentHighlight_2 = (currentHighlight_2 + 1) % strip.numPixels();
        } else if (ppl_D < 0) {
            // Move left
            currentHighlight_1 = (currentHighlight_1 - 1 + strip.numPixels()) % strip.numPixels();
            currentHighlight_2 = (currentHighlight_2 - 1 + strip.numPixels()) % strip.numPixels();
        }
        // When ppl_D == 0, highlights stay in place
        lastHighlightUpdate = millis();
    }
}

int LEDModes::getColorDelay() {
    return colorDelayTime;
}

int LEDModes::getHighlightDelay() {
    return highlightDelayTime;
}

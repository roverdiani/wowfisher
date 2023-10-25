/* 
  This project is intended only as a learning exercise.
  Don’t use it to take advantage in the game, and don’t try it on Blizz’s live servers.
  Automation of gameplay is against the rules, and you could lose your account. 
  Try it on a private server if you wish to see it working.
*/

#include <Keyboard.h>

#define SECOND 1000
#define MINUTE (SECOND * 60)

#define FISHING_TIMEOUT (30 * SECOND)
#define FISHING_SOUND_THRESHOLD 30

const char FISHING_KEY = '1';
const char CATCH_KEY = 'f';

// Variable to keep track if we are fishing or not
bool isFishing = true;

// Throw the fishing line by issuing the fishing key keystroke
void fish()
{
  Serial.println("Fishing.");
  Keyboard.write(FISHING_KEY);
  isFishing = true;
}

// Wait between minDelay and maxDelay and then catch the fish by issuing the catch keystroke.
void catch_fish(int minDelay, int maxDelay)
{
  // Ignore this if we are not currently fishing.
  // This exists so that it doesnt issue multiple keystrokes when already catching a fish.
  if (!isFishing)
  {
    Serial.println("CATCH: called when not fishing. Ignoring.");
    return;
  }

  // Wait sometime before catching the fish.
  int delayTime = random(minDelay, maxDelay);
  Serial.print("CATCH: delay ");
  Serial.print(delayTime);
  Serial.println("ms before catching.");
  delay(delayTime);

  Keyboard.write(CATCH_KEY);
  isFishing = false;
}

void setup()
{
  delay(1 * SECOND);

  // Init the keyboard and serial libraries.
  Keyboard.begin();
  Serial.begin(9600);
  Serial.println("Fisher v0.1");

  delay(1 * SECOND);
}

void loop()
{
  int soundDetect;

  fish();
  unsigned long fishing_start_time = millis();

  // Keep listening for the sound the game makes when a fish is hooked.
  do
  {
    // If enough time has elapsed, maybe we missed some fish, so it starts again.
    if (millis() > (fishing_start_time + FISHING_TIMEOUT))
    {
      Serial.println("LOOP: failed to detect catch sound. Starting again.");
      fish();
      fishing_start_time = millis();
    }

    soundDetect = analogRead(A1);
    delay(100);
  } while (soundDetect < FISHING_SOUND_THRESHOLD);

  // We caught some fish! Yay!
  Serial.println("Fish detected.");
  catch_fish(1000, 1500);

  // Wait a little before throwing the line again.
  int delay_time = random(1500, 4000);
  Serial.print("LOOP: wait ");
  Serial.print(delay_time);
  Serial.println("ms before fishing again.");
  delay(delay_time);
}

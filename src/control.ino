#include <ESP8266WiFi.h>
#include <SPI.h>
#include <EEPROM.h>
#include "UserStorage.hpp"
#include "MFRC522.h"

#define RST_PIN	15
#define SS_PIN	2

const char *ssid = "HaSi-Kein-Internet-Legacy";
const char *pass = "bugsbunny";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
    Users.begin();
    Serial.begin(115200);
    SPI.begin();

    mfrc522.PCD_Init();

    WiFi.begin(ssid, pass);

    pinMode(16, OUTPUT);

    int retries = 0;

    while ((WiFi.status() != WL_CONNECTED) && (retries < 10))
    {
        retries++;
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi connected!");
    }

    Serial.println("Ready!");

    Users.printUsers();
}

void loop()
{
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        delay(50);
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial())
    {
        delay(50);
        return;
    }

    int addUserResult = Users.addUser(mfrc522.uid.uidByte);

    if (addUserResult == ALREADY_REGISTERED)
    {
        int removeUserResult = Users.removeUser(mfrc522.uid.uidByte);

        if (removeUserResult == SUCCESSFULLY_REMOVED)
        {
            Serial.println("Removed uid");
        }
        else
        {
            Serial.println("Something went wrong");
        }
    }
    else if (addUserResult == SLOTS_EXCEEDED)
    {
        Serial.println("No more free slots");
    }
    else
    {
        Serial.println("Added uid");
    }

    mfrc522.PICC_HaltA();

    /*digitalWrite(16, HIGH);
    delay(1000);
    digitalWrite(16, LOW);
    delay(1000);*/
}

#ifndef USER_STORAGE_HPP
#define USER_STORAGE_HPP

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 4096
#define SLOT_SIZE   12
#define UID_SIZE    7

#define FLAGS_BYTE  11
#define USED_FLAG   0x01
#define ADMIN_FLAG  0x02

#define SLOTS_COUNT EEPROM_SIZE / SLOT_SIZE

#define ALREADY_REGISTERED   -1
#define SLOTS_EXCEEDED       -2
#define SUCCESSFULLY_ADDED   0
#define NO_USER_TO_REMOVE    -1
#define SUCCESSFULLY_REMOVED 0

class UserStorage
{
public:
    UserStorage();
    ~UserStorage();

    void begin();
    void end();

    void printUsers();

    int addUser(byte *uid);
    int removeUser(byte *uid);

private:
    bool getFlag(int slot, int flag);
    void setFlag(int slot, int flag, bool value);

    void readSlots();
    void writeSlot(int slot);
    void writeSlots();

    void printBytes(byte *buffer, byte bufferSize);

private:
    byte _slots[SLOTS_COUNT][SLOT_SIZE];
};

extern UserStorage Users;

#endif

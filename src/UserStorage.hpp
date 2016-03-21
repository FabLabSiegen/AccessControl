#ifndef USER_STORAGE_HPP
#define USER_STORAGE_HPP

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 4096
#define SLOT_SIZE   12
#define UID_SIZE    7

#define TOTAL_SLOTS     8 * EEPROM_SIZE / (1 + 8 * UID_SIZE)
#define SLOT_INDEX_SIZE TOTAL_SLOTS / 8
#define SLOTS_SIZE      TOTAL_SLOTS * SLOT_SIZE

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
    void readSlotIndex();
    void writeSlotIndexForSlot(int slot);
    void writeSlotIndex();

    void readSlots();
    void writeSlotsForSlot(int slot);
    void writeSlots();

    void printBytes(byte *buffer, byte bufferSize);

private:
    bool _slotIndex[TOTAL_SLOTS];
    byte _slots[TOTAL_SLOTS][SLOT_SIZE];
};

extern UserStorage Users;

#endif

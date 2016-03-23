#include "UserStorage.hpp"

UserStorage::UserStorage()
    : _slots{ { 0x00, }, }
{
}

UserStorage::~UserStorage()
{
}

void UserStorage::begin()
{
    EEPROM.begin(EEPROM_SIZE);

    readSlots();
}

void UserStorage::end()
{
    EEPROM.end();
}

void UserStorage::printUsers()
{
    for (int i = 0; i < SLOTS_COUNT; i++)
    {
        if (getFlag(i, USED_FLAG))
        {
            Serial.print("Slot ");
            Serial.print(i);
            Serial.print(": ");

            printBytes(_slots[i], UID_SIZE);

            Serial.println();
        }
    }
}

int UserStorage::addUser(byte *uid)
{
    // alreay in list?
    for (int i = 0; i < SLOTS_COUNT; i++)
    {
        bool sameSame = true;

        for (int j = 0; j < UID_SIZE; j++)
        {
            if (_slots[i][j] != uid[j])
            {
                sameSame = false;
            }
        }

        if (sameSame)
        {
            return ALREADY_REGISTERED;
        }
    }

    int freeSlot = -1;

    // find next free slot
    for (int i = 0; i < SLOTS_COUNT; i++)
    {
        if (!getFlag(i, USED_FLAG))
        {
            freeSlot = i;
        }
    }

    if (freeSlot == -1)
    {
        return SLOTS_EXCEEDED;
    }

    setFlag(freeSlot, USED_FLAG, true);

    for (int i = 0; i < UID_SIZE; i++)
    {
        _slots[freeSlot][i] = uid[i];
    }

    writeSlot(freeSlot);

    return SUCCESSFULLY_ADDED;
}

int UserStorage::removeUser(byte *uid)
{
    // alreay in list?
    for (int i = 0; i < SLOTS_COUNT; i++)
    {
        bool sameSame = true;

        for (int j = 0; j < UID_SIZE; j++)
        {
            if (_slots[i][j] != uid[j])
            {
                sameSame = false;
            }
        }

        if (sameSame)
        {
            setFlag(i, USED_FLAG, false);

            for (int j = 0; i < UID_SIZE; j++)
            {
                _slots[i][j] = 0x00;

                writeSlot(i);
            }

            return SUCCESSFULLY_REMOVED;
        }
    }

    return NO_USER_TO_REMOVE;
}

bool UserStorage::getFlag(int slot, int flag)
{
    return bool(_slots[slot][FLAGS_BYTE] & flag);
}

void UserStorage::setFlag(int slot, int flag, bool value)
{
    _slots[slot][FLAGS_BYTE] &= ~flag;
    _slots[slot][FLAGS_BYTE] |= value ? flag : 0x00;
}

void UserStorage::readSlots()
{
    for (int i = 0; i < SLOTS_COUNT; i++)
    {
        _slots[i / SLOT_SIZE][i % SLOT_SIZE] = EEPROM.read(i);
    }
}

void UserStorage::writeSlot(int slot)
{
    for (int i = 0; i < SLOTS_COUNT * SLOT_SIZE; i++)
    {
        EEPROM.write(slot * SLOT_SIZE + i, _slots[slot][i]);
    }
}

void UserStorage::writeSlots()
{
    for (int i = 0; i < SLOTS_COUNT * SLOT_SIZE; i++)
    {
        EEPROM.write(i, _slots[i / SLOT_SIZE][i % SLOT_SIZE]);
    }
}

void UserStorage::printBytes(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

UserStorage Users;

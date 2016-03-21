#include "UserStorage.hpp"

UserStorage::UserStorage()
    : _slotIndex{ false, },
      _slots{ { 0x00, }, }
{
}

UserStorage::~UserStorage()
{
}

void UserStorage::begin()
{
    EEPROM.begin(EEPROM_SIZE);

    readSlotIndex();
    readSlots();
}

void UserStorage::end()
{
    EEPROM.end();
}

void UserStorage::printUsers()
{
    for (int i = 0; i < TOTAL_SLOTS; i++)
    {
        if (_slotIndex[i])
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
    for (int i = 0; i < TOTAL_SLOTS; i++)
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
    for (int i = 0; i < TOTAL_SLOTS; i++)
    {
        if (!_slotIndex[i])
        {
            freeSlot = i;
        }
    }

    if (freeSlot == -1)
    {
        return SLOTS_EXCEEDED;
    }

    _slotIndex[freeSlot] = true;

    for (int i = 0; i < UID_SIZE; i++)
    {
        _slots[freeSlot][i] = uid[i];
    }

    writeSlotIndexForSlot(freeSlot);
    writeSlotsForSlot(freeSlot);

    return SUCCESSFULLY_ADDED;
}

int UserStorage::removeUser(byte *uid)
{
    // alreay in list?
    for (int i = 0; i < TOTAL_SLOTS; i++)
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
            _slotIndex[i] = false;

            for (int j = 0; i < UID_SIZE; j++)
            {
                _slots[i][j] = 0x00;

                writeSlotIndexForSlot(i);
                writeSlotsForSlot(i);
            }

            return SUCCESSFULLY_REMOVED;
        }
    }

    return NO_USER_TO_REMOVE;
}

void UserStorage::readSlotIndex()
{
    for (int i = 0; i < SLOT_INDEX_SIZE; i++)
    {
        byte eepromValue = EEPROM.read(i);

        for (int j = 0; j < 8; j++)
        {
            _slotIndex[i * 8 + j] = bool(eepromValue >> j);
        }
    }
}

void UserStorage::writeSlotIndexForSlot(int slot)
{
    byte newEepromValue = 0x00;

    for (int j = 0; j < 8; j++)
    {
        newEepromValue |= (_slotIndex[slot - slot % 8 + j] ? 1 : 0) << j;
    }

    EEPROM.write(SLOT_INDEX_SIZE + slot * SLOT_SIZE, newEepromValue);

    EEPROM.commit();
}

void UserStorage::writeSlotIndex()
{
    for (int i = 0; i < SLOT_INDEX_SIZE; i++)
    {
        byte newEepromValue = 0x00;

        for (int j = 0; j < 8; j++)
        {
            newEepromValue |= (_slotIndex[i * 8 + j] ? 1 : 0) << j;
        }

        EEPROM.write(i, newEepromValue);
    }

    EEPROM.commit();
}

void UserStorage::readSlots()
{
    for (int i = 0; i < SLOTS_SIZE; i++)
    {
        _slots[i / SLOT_SIZE][i % SLOT_SIZE] = EEPROM.read(SLOT_INDEX_SIZE + i);
    }
}

void UserStorage::writeSlotsForSlot(int slot)
{
    for (int i = 0; i < SLOT_SIZE; i++)
    {
        EEPROM.write(SLOT_INDEX_SIZE + slot * SLOT_SIZE + i, _slots[slot][i]);
    }
}

void UserStorage::writeSlots()
{
    for (int i = 0; i < SLOTS_SIZE; i++)
    {
        EEPROM.write(SLOT_INDEX_SIZE + i, _slots[i / SLOT_SIZE][i % SLOT_SIZE]);
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

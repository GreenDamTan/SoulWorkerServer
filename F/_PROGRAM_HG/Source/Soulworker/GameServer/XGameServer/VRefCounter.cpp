// VRefCounter.cpp
// VRefCounter implementation
// IDA decompilation from GameServer.exe

#include "VRefCounter.h"

// ============================================================================
// VRefCounter::DeleteThis - Delete this object
// IDA @ 0x140188D60
// ============================================================================
void VRefCounter::DeleteThis()
{
    // IDA: void __fastcall VRefCounter::DeleteThis(VRefCounter *this)
    // {
    //   if ( this )
    //     ((void (__fastcall *)(VRefCounter *, __int64))this->dtr_VBaseObject)(this, 1);
    // }

    if (this)
    {
        // Call virtual destructor with delete flag (1)
        // This triggers the virtual destructor chain which deletes the object
        delete this;
    }
}

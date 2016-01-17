/*
 * Copyright (c) 2008-2016, Integrity Project Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of the Integrity Project nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE
 */

/*
 * tss.cpp
 *
 * Implementation file
 *
 * Author: Elad Raz <e@eladraz.com>
 */
#include "xStl/types.h"
#include "xStl/except/trace.h"
#include "xdk/hooker/Processors/ia32/registers.h"
#include "xdk/hooker/Processors/ia32/segments.h"
#include "xdk/hooker/Processors/ia32/tss32.h"

TaskStateSegment32* TaskStateSegment::getTSS32(uint16 selector)
{
    Selector sel(selector);
    CHECK(sel.isGDTselector());
    GDTR gdt;
    SegmentDescriptor tss(gdt, sel);
    CHECK(tss.isSystem());
    CHECK((tss.getSystemSegmentType() == SegmentDescriptor::SEGMENT_TSS32_FREE) ||
          (tss.getSystemSegmentType() == SegmentDescriptor::SEGMENT_TSS32_BUSY));
    CHECK(tss.getSegmentLimit() >= sizeof(TaskStateSegment32));
    return (TaskStateSegment32*)getPtr(tss.getOffset());
}

uint16 TaskStateSegment::getTaskRegister()
{
    uint16 returnCode;
    // Get the opcode
    _asm { str returnCode };
    return returnCode;
}
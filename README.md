# addr2js

This is a node.js module that tracks JS function code events and logs them
to disk, to a file called `/tmp/addr2js-<pid>.map`.

The generated output looks like this:

    A 3e888e506060 108 Stub:CompareICStub
    A 3e888e506140 698 Stub:CEntryStub
    A 3e888e506460 72 Stub:StoreBufferOverflowStub
    A 3e888e506520 676 Stub:RecordWriteStub
    A 3e888e506840 77 Stub:CompareICStub
    A 3e888e506900 136 Stub:FastCloneShallowArrayStub
    A 3e888e506a00 118 Stub:CompareICStub
    A 3e888e506ae0 1654 Stub:RecordWriteStub
    A 3e888e5071c0 42 Stub:ToBooleanStub
    A 3e888e507260 43 Stub:ToNumberStub

Where 'A' means added, 'M' means moved and 'R' means removed.  The function
name is only logged for A records.  M records log the new function address
instead.

This module is primarily intended to be used in conjunction with tools such as
[perf(1)](https://perf.wiki.kernel.org/).  Note that no direct integration is
provided, this module is purely for collecting the raw data.

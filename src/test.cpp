#include "stream_reassembler.hh"
#include "byte_stream.hh"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
    StreamReassembler reassembler(15);

    reassembler.push_substring("ABC", 3, false);
    reassembler.push_substring("123", 0, false);
    reassembler.push_substring("XYZ", 12, false);
    reassembler.push_substring("456", 9, false);

    assert(reassembler.unassembled_bytes() == 6);

    assert(reassembler.ack_index() == 6);

    reassembler.push_substring("DEF", 6, false);
    reassembler.push_substring("789", 1, false);

    assert(reassembler.ack_index() == 15);

    assert(reassembler.empty());

    reassembler.push_substring("", 15, true);

    assert(reassembler.empty());

    ByteStream output = reassembler.stream_out();
    assert(output.input_ended());

    assert(output.peek_output(15) == "123ABCDEF456XYZ");

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

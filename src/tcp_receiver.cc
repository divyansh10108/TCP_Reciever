#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;
size_t buffer_size;
size_t c;
void TCPReceiver::segment_received(const TCPSegment &seg) {
    buffer_size = _reassembler.stream_out().buffer_size(); 
    const TCPHeader head = seg.header();
    auto seq  =head.seqno;
    if (!_synReceived) {
        _isn = seq;
        _synReceived = !_synReceived;
    }
    // ...
    // --- Hint ------
        // convert the seqno into absolute seqno
    uint64_t checkpoint = _reassembler.ack_index();
    c = checkpoint;
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno -_synReceived;
    auto payl = seg.payload().copy();
    // --- Hint ------  
	_reassembler.push_substring(payl,stream_idx,_finReceived = head.fin);
    // ... 
}


optional<WrappingInt32> TCPReceiver::ackno() const {
    buffer_size = _reassembler.stream_out().buffer_size(); 
    c = _reassembler.ack_index()+1;
    if(_synReceived){ 
    	return _reassembler.stream_out().input_ended() ? WrappingInt32(_isn) + 	c :WrappingInt32(_isn) + c-1;
    }
    else{
    	return nullopt;
    }
    
}

size_t TCPReceiver::window_size() const { 
	buffer_size = _reassembler.stream_out().buffer_size(); 
	return _capacity - buffer_size; 
}

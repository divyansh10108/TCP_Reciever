#include "wrapping_integers.hh"

#include <iostream>

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) { 
return isn + uint32_t(n); }

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint32_t offset = n.raw_value() - isn.raw_value();
    int condi  =0;
    uint64_t x =checkpoint - offset;
    uint64_t twop = 1;
    for(int i = 0;i<32;i++){
    	twop*= 2;
    }
    uint64_t a = x;
    a/=twop;
    a*=twop;
    a+=offset; 
    if(x + offset > offset){
    	return checkpoint>= (2*a+twop)/2? a+twop:a;
    }else{
        return offset;
    }
     
    
    
}

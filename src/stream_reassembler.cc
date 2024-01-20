#include "stream_reassembler.hh"

#include <iostream>

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;
StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), firstunassembled(0), firstunread(0), m(), flag(capacity, false), length(-1), count(0), lastassembleed(0) {}
//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
size_t datalen;
string s;
bool checkValid(size_t ind,size_t firstunassembled,size_t _capacity,vector<bool> flag){
	return ind < firstunassembled || flag[ind%_capacity];
}
size_t reassemble(vector<bool> &flag,size_t &firstunassembled,size_t &firstunread,size_t &_capacity,size_t &length,map<size_t,char> &m,size_t &count){
	size_t i = firstunassembled;
	
	while(i - firstunread<_capacity){
    		int val = i%_capacity;
        	if (flag[val]){
        		count--;
        		s += m[val];
      		  	flag[val] = 0; 	
       
      	 	}else{
       		     break;
 		}
 		i++;
       }
    return i;
}
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    s = "";
    firstunread = _output.bytes_read();
    datalen = data.size();
    if (eof)
        length = index + data.size();
    size_t i = index;
    while(i < index + data.size() && i < firstunread + _capacity){
    	size_t  fir = i - index;
    	int val = i%_capacity;
        if (checkValid(i,firstunassembled,_capacity,flag)){
        	;
        }
        else{
		lastassembleed = max(lastassembleed , fir + index);
		flag[(fir+index)%_capacity] =1;
		m[(fir+index)%_capacity] = data[fir];
		count+=1;
        }
        i++;
    }
    
    firstunassembled = reassemble(flag,firstunassembled,firstunread,_capacity,length,m,count);
    if (firstunassembled == length)
        _output.end_input();

    if (s!= "")
        _output.write(s);
}

size_t StreamReassembler::unassembled_bytes() const { return size_t(count); }

bool StreamReassembler::empty() const {
    return count == 0;
}
size_t StreamReassembler::ack_index() const {return _output.bytes_written() +1;}




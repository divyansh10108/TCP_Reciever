#include "byte_stream.hh"
#include <algorithm>

ByteStream::ByteStream(const size_t capa)
    : capacity_(capa), endOfInput(false), error(false),reader(0),writter(0) {
    // No need to initialize queue, it starts empty
}

size_t ByteStream::write(const std::string &data) {

    const size_t availableSpace = capacity_ - buffer.size();
    const size_t bytesToWrite = std::min(data.size(), availableSpace);

    for (size_t i = 0; i < bytesToWrite; ++i) {
        buffer.push_back(data[i]);
        writter++;
    }

    return bytesToWrite;
}

size_t ByteStream::remaining_capacity() const {

    return capacity_ - buffer.size();
}

void ByteStream::end_input() {
    endOfInput = true;
}

std::string ByteStream::peek_output(const size_t len) const {
    if (buffer.empty()) {
        return ""; // Nothing to peek, buffer is empty
    }

    std::string result;
    size_t bytesToPeek = std::min(len, buffer.size());

    std::deque<char> tempBuffer = buffer; // Create a temporary copy to avoid modifying the original
    for (size_t i = 0; i < bytesToPeek; ++i) {
        result += tempBuffer.front();
        tempBuffer.pop_front();
        
    }

    return result;
}

void ByteStream::pop_output(const size_t len) {
    if (buffer.empty()) {
    	//error = true;
        return; // Nothing to pop, buffer is empty
    }
    size_t bytesToPop = std::min(len, buffer.size());
    if (len>buffer.size()) {
    	error = true;
    	return;
    }
    for (size_t i = 0; i < bytesToPop; ++i) {
        buffer.pop_front();
        reader++;
    }
}
std::string ByteStream::read(const size_t len) {
    if (buffer.empty() && !endOfInput) {
    	//error = true;
        return ""; // Nothing to read, buffer is empty, and input is not ended
    }

    std::string result;
    size_t bytesToRead = std::min(len, buffer.size());
    if(len> buffer.size()){
    	error = true;
    	return "";
    }

    for (size_t i = 0; i < bytesToRead; ++i) {
        result += buffer.front();
        buffer.pop_front();
        reader++;
    }

    return result;
}

bool ByteStream::input_ended() const {
    return endOfInput;
}

size_t ByteStream::buffer_size() const {
    return buffer.size();
}

bool ByteStream::buffer_empty() const {
    return buffer.empty();
}

bool ByteStream::eof() const {
    return endOfInput && buffer_empty();
}

size_t ByteStream::bytes_written() const {
    return writter;
}

size_t ByteStream::bytes_read() const {
    return reader;
}


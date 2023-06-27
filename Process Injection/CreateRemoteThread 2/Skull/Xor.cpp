#include <iostream>
#include <cstring>

using namespace std;
// POC of a speedy XOR function, tbd pass in unsigned char* shellcode (pointer) into XOR function and pray that i know how pointer references work
struct byte_array {
    unsigned char* data;
    int size;

    byte_array(int size) {
        this->size = size;
        data = new unsigned char[size];
    }

    byte_array(const byte_array& other) {
        size = other.size;
        data = new unsigned char[size];
        memcpy(data, other.data, size);
    }

    ~byte_array() {
        delete[] data;
    }
};

void xor_byte_arrays(const byte_array& input, const byte_array& key, byte_array& output) {
    if (input.size != key.size) {
        cerr << "Error: input and key arrays must be the same length." << endl;
        return;
    }

    output = byte_array(input.size);

    for (int i = 0; i < input.size; i++) {
        output.data[i] = input.data[i] ^ key.data[i];
    }
}

int main() {
    byte_array input(4);
    input.data[0] = 0x12;
    input.data[1] = 0x34;
    input.data[2] = 0x56;
    input.data[3] = 0x78;

    byte_array key(4);
    key.data[0] = 0xab;
    key.data[1] = 0xcd;
    key.data[2] = 0xef;
    key.data[3] = 0x12;

    byte_array output(4);

    xor_byte_arrays(input, key, output);

    cout << "Input: ";
    for (int i = 0; i < input.size; i++) {
        printf("%02x ", input.data[i]);
    }
    cout << endl;

    cout << "Key: ";
    for (int i = 0; i < key.size; i++) {
        printf("%02x ", key.data[i]);
    }
    cout << endl;

    cout << "Output: ";
    for (int i = 0; i < output.size; i++) {
        printf("%02x ", output.data[i]);
    }
    cout << endl;

    return 0;
}

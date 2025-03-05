#include "HCTree.hpp"
#include "Helper.hpp"

int main(int argc, char *argv[]){
    if(argc != 3){
        cerr << "Usage: ./compress <original_file> <compressed_file>" << endl;
        return 1; 
    }

    // parse args
    string original_file = argv[1];
    string compressed_file = argv[2];

    FancyInputStream fancy_input(original_file.c_str());
    if(!fancy_input.good()){
        cerr << "Could not open input file: " << original_file << endl;
        return 1;
    }
    FancyOutputStream fancy_output(compressed_file.c_str());
    if(!fancy_output.good()){
        cerr << "Could not open output file: " << compressed_file << endl;
        return 1;
    }
    cout << "Opened files: " << original_file << ", " << compressed_file << endl;

    // initialize a vector
    vector<int> freqs(256, 0);
    int byte;
    while((byte = fancy_input.read_byte()) != -1){
        freqs[(unsigned int)byte]++;
    }

    // check for empty file
    bool empty_checker = true;
    for(int f : freqs){
        if(f > 0){
            empty_checker = false;
            break;
        }
    }
    if(empty_checker == true){
        cerr << "Input file is empty." << endl;
        return 0;
    }

    // initialize new huffman tree and build with freqs
    HCTree huffman_tree;
    huffman_tree.build(freqs);

    int total_symbols = 0;
    for(size_t i = 0; i < freqs.size(); i++){
        fancy_output.write_int(freqs[i]);
        total_symbols += freqs[i];
    }
    fancy_output.write_int(total_symbols);

    fancy_input.reset();

    while((byte = fancy_input.read_byte()) != -1){
        cout << "Encoding byte: " << byte << endl;
        huffman_tree.encode((unsigned char)byte, fancy_output);
    }

    fancy_output.flush();
    cout << "Compressed file completed." << endl;

    return 0;
}
#include "HCTree.hpp"
#include "Helper.hpp"

int main (int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: ./uncompress <compressed_file> <uncompressed_file>" << endl;
        return 1;
    }

    string compressed_file = argv[1];
    string uncompressed_file = argv[2];
    
    FancyInputStream fancy_input(compressed_file.c_str());
    if(!fancy_input.good()){
        cerr << "Could not open input file: " << compressed_file << endl;
        return 1;
    }
    FancyOutputStream fancy_output(uncompressed_file.c_str());
    if(!fancy_output.good()){
        cerr << "Could not open output file: " << uncompressed_file << endl;
        return 1;
    }
    cout << "Opened files: " << compressed_file << ", " << uncompressed_file << endl;
    
    vector<int> freqs(256, 0);
    for(size_t i = 0; i < freqs.size(); i++){
        freqs[i] = fancy_input.read_int();
    }

    bool empty_checker = true;
    for(int f : freqs){
        if(f > 0){
            empty_checker = false;
            break;
        }
    }
    if(empty_checker == true){
        cerr << "Compressed file is empty." << endl;
        return 0;
    }

    HCTree huffman_tree;
    huffman_tree.build(freqs);

    int total_symbols = fancy_input.read_int();
    int symbols_decoded = 0;

    int decode_byte;
    int last_byte = -1;
    while((symbols_decoded < total_symbols) && (decode_byte = huffman_tree.decode(fancy_input)) != EOF){
        fancy_output.write_byte((unsigned int)decode_byte);
        last_byte = decode_byte;
        symbols_decoded++;
    }
    if(last_byte == '\n'){
        fancy_output.write_byte('\n');
    }

    fancy_output.flush();
    cout << "Uncompressed file completed." << endl;
    
    return 0;
}

#include <fstream>
#include <vector>
#include "ArraySequence.h"
#include "ListSequence.h"

template<typename T>
void print_sequence(const Sequence<T>* seq, const std::string& name, std::ostream& out) {
    out << name << " (size: " << seq->GetLength() << "): [";
    for (int i = 0; i < seq->GetLength(); i++) {
        out << seq->Get(i) << (i < seq->GetLength() - 1 ? ", " : "");
    }
    out << "]\n";
}

std::vector<int> read_data(std::ifstream& in, int count) {
    std::vector<int> data;
    data.reserve(count);
    for (int i = 0; i < count; i++) {
        int value;
        if (!(in >> value)) throw std::runtime_error("Invalid input data");
        data.push_back(value);
    }
    return data;
}

void run_sequence_tests(const std::string& input_file, const std::string& output_file) {
    std::ifstream in(input_file);
    std::ofstream out(output_file);

    if (!in.is_open()) throw std::runtime_error("Cannot open input file");
    if (!out.is_open()) throw std::runtime_error("Cannot open output file");

   
    int arr_size, list_size;
    in >> arr_size >> list_size;
    auto arr_data = read_data(in, arr_size);
    auto list_data = read_data(in, list_size);
  
    int manual_data[] = {55,66,77};
    MutableArraySequence<int> arr0(manual_data, 3); 
    MutableArraySequence<MutableArraySequence<int>> arr;  
    arr.Append(arr0);
    out << "0 0 element: " << arr.Get(0).Get(2) << "\n";
   
    out << " Testing MutableArraySequence \n";
    MutableArraySequence<int> mutable_arr(arr_data.data(), arr_size);
    print_sequence(&mutable_arr, "Initial MutableArray", out);

    mutable_arr.Append(10)->Prepend(5)->InsertAt(7, 1);
    print_sequence(&mutable_arr, "Modified MutableArray", out);

    out << "\n Testing ImmutableArraySequence \n";
    ImmutableArraySequence<int> immutable_arr(arr_data.data(), arr_size);
    print_sequence(&immutable_arr, "Original ImmutableArray", out);

    auto modified_arr = immutable_arr.Append(4);
    print_sequence(&immutable_arr, "Original After Append", out);
    print_sequence(modified_arr, "New ImmutableArray", out);

    
    out << "\n     Testing MutableListSequence \n";
    MutableListSequence<int> mutable_list(list_data.data(), list_size);
    mutable_list.Append(100)->Prepend(50);
    print_sequence(&mutable_list, "Modified MutableList", out);

    out << "\n Testing ImmutableListSequence \n";
    ImmutableListSequence<int> immutable_list(list_data.data(), list_size);
    print_sequence(&immutable_list, "Original ImmutableList", out);

    auto new_list = immutable_list.Append(40);
    print_sequence(&immutable_list, "Original After Append", out);
    print_sequence(new_list, "New ImmutableList", out);


    mutable_arr.Concat(&immutable_list);
    print_sequence(&mutable_arr, "Concat", out);

  
   

    
}
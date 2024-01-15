#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>


using namespace std;
using namespace std::string_literals;

struct Nucleotide {
    char symbol;
    size_t position;
    int chromosome_num;
    int gene_num;
    bool is_marked;
    char service_info;
};
const size_t N = 4;
const std::array<char, N> letters = {'A', 'G', 'T', 'C'};

struct CompactNucleotide {
    uint32_t position; //1-2
    uint16_t gene_num:15; //3-4: 0-14
    uint16_t is_marked:1; //4: 15
    uint16_t service_info:8;
    uint16_t symbol:2; // 5: 0-1
    uint16_t chromosome_num:6; // 5: 2-7

};

CompactNucleotide Compress(const Nucleotide& n) {
    // напишите вашу реализацию здесь
    CompactNucleotide res;
    res.chromosome_num = n.chromosome_num;
    res.gene_num = n.gene_num;
    res.position = static_cast<uint32_t>(n.position);
    res.is_marked = n.is_marked;
    res.service_info = n.service_info;
    for (size_t i =0; i<N; ++i) {
        if (n.symbol == letters[i]) {
            res.symbol = i;
            continue;
        }
    }

    return res;
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    // напишите вашу реализацию здесь
    Nucleotide res;
    res.symbol = letters.at(cn.symbol);
    res.position = cn.position;
    res.is_marked = cn.is_marked;
    res.chromosome_num = cn.chromosome_num;
    res.gene_num = cn.gene_num;
    res.service_info = cn.service_info;
    return res;
}

static_assert(sizeof(CompactNucleotide) <= 8);//, "Your CompactNucleotide is not compact enough"s);
static_assert(alignof(CompactNucleotide) == 4);//, "Don't use '#pragma pack'!"s);
bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.symbol == rhs.symbol) && (lhs.position == rhs.position) && (lhs.chromosome_num == rhs.chromosome_num)
        && (lhs.gene_num == rhs.gene_num) && (lhs.is_marked == rhs.is_marked) && (lhs.service_info == rhs.service_info);
}
void TestSize() {
    assert(sizeof(CompactNucleotide) <= 8);
}
void TestCompressDecompress() {
    Nucleotide source;
    source.symbol = 'T';
    source.position = 1'000'000'000;
    source.chromosome_num = 48;
    source.gene_num = 1'000;
    source.is_marked = true;
    source.service_info = '!';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);

    assert(source == decompressedSource);
}

int main() {
    TestSize();
    TestCompressDecompress();
}
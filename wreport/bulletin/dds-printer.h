#ifndef WREPORT_BULLETIN_DDS_PRINTER_H
#define WREPORT_BULLETIN_DDS_PRINTER_H

#include <wreport/bulletin.h>
#include <wreport/bulletin/internals.h>
#include <vector>
#include <cstdio>

namespace wreport {
namespace bulletin {

/**
 * bulletin::Visitor that prints the bulletin contents and its structure
 */
class DDSPrinter : public BaseParser
{
    std::vector<Varcode> stack;
    FILE* out;

    void print_context(Varinfo info, unsigned var_pos);
    void print_context(Varcode code, unsigned var_pos);
    void print_attr(Varinfo info, unsigned var_pos);

public:
    /**
     * Create a new DDS printer
     *
     * @param b
     *   Reference to the bulletin being visited
     * @param out
     *   FILE to print to
     */
    DDSPrinter(Bulletin& b, FILE* out, unsigned subset_idx);
    virtual ~DDSPrinter();

    void define_substituted_value(unsigned pos) override;
    void define_attribute(Varinfo info, unsigned pos) override;
    void define_variable(Varinfo info) override;
    const Var& define_semantic_variable(Varinfo info) override;
    void define_bitmap(Varcode rep_code, Varcode delayed_code, const Opcodes& ops) override;
    void define_raw_character_data(Varcode code) override;

    void r_replication(Varcode code, Varcode delayed_code, const Opcodes& ops) override;
    void d_group_begin(Varcode code) override;
    void d_group_end(Varcode code) override;
};

}
}
#endif

/*
 * bulletin/dds-printer - Print a DDS using the interpreter
 *
 * Copyright (C) 2011  ARPA-SIM <urpsim@smr.arpa.emr.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * Author: Enrico Zini <enrico@enricozini.com>
 */

#include "wreport/bulletin/dds-printer.h"

using namespace std;

namespace wreport {
namespace bulletin {

DDSPrinter::DDSPrinter(const Bulletin& b, FILE* out)
    : ConstBaseVisitor(b), out(out)
{
}

DDSPrinter::~DDSPrinter() {}

void DDSPrinter::print_context(Varinfo info, unsigned var_pos)
{
    print_context(info->var, var_pos);
}

void DDSPrinter::print_context(Varcode code, unsigned var_pos)
{
    fprintf(out, "%2u.%2u ", current_subset_no, var_pos);
    for (vector<Varcode>::const_iterator i = stack.begin();
            i != stack.end(); ++i)
        fprintf(out, "%01d%02d%03d/", WR_VAR_F(*i), WR_VAR_X(*i), WR_VAR_Y(*i));
    fprintf(out, "%01d%02d%03d: ", WR_VAR_F(code), WR_VAR_X(code), WR_VAR_Y(code));
}

void DDSPrinter::d_group_begin(Varcode code)
{
    ConstBaseVisitor::d_group_begin(code);
    stack.push_back(code);
}

void DDSPrinter::d_group_end(Varcode code)
{
    ConstBaseVisitor::d_group_end(code);
    stack.pop_back();
}

void DDSPrinter::r_replication(Varcode code, Varcode delayed_code, const Opcodes& ops)
{
    stack.push_back(code);
    ConstBaseVisitor::r_replication(code, delayed_code, ops);
    stack.pop_back();
}

void DDSPrinter::do_start_subset(unsigned subset_no, const Subset& current_subset)
{
    ConstBaseVisitor::do_start_subset(subset_no, current_subset);
    stack.clear();
}

void DDSPrinter::do_attr(Varinfo info, unsigned var_pos, Varcode attr_code)
{
    print_context(info, var_pos);
    fprintf(out, "(attr)");

    const Var& var = get_var(var_pos);
    if (const Var* a = var.enqa(attr_code))
        a->print(out);
    else
        fprintf(out, "(undef)");
}

void DDSPrinter::do_var(Varinfo info)
{
    print_context(info, current_var);

    const Var& var = get_var();
    var.print(out);
}

void DDSPrinter::do_associated_field(unsigned bit_count, unsigned significance)
{
    const Var& var = get_var(current_var);
    print_context(var.info(), current_var);

    const Var* att = var.enqa_by_associated_field_significance(significance);
    if (att)
        att->print(out);
    else
        fprintf(out, "associated field with significance %d is not present", significance);
}

Var DDSPrinter::do_semantic_var(Varinfo info)
{
    const Var& var = get_var();
    var.print(out);
    return var;
}

const Var* DDSPrinter::do_bitmap(Varcode code, Varcode delayed_code, const Opcodes& ops)
{
    const Var* res = ConstBaseVisitor::do_bitmap(code, delayed_code, ops);
    if (delayed_code)
    {
        Varinfo info = btable->query(delayed_code);
        print_context(info, 0);
        Var var(info, (int)res->info()->len);
        var.print(out);
    }
    print_context(res->info(), 0);
    res->print(out);
    return res;
}

void DDSPrinter::do_char_data(Varcode code)
{
    print_context(code, 0);

    const Var& var = get_var();
    var.print(out);
}

}
}

/* vim:set ts=4 sw=4: */

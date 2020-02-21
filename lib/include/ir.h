#ifndef AOSC_DK_IR_H
#define AOSC_DK_IR_H

int dk_ir_parse(const char *ir);
int dk_ir_parse_gvariant(GVariant *ir);

int dk_ir_emit(char **ir);
int dk_ir_emit_gvariant(GVariant *ir);

int dk_ir_get(const char *property, char **out);
int dk_ir_set(const char *property, const char *in);

#endif

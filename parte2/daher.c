/*
  Pro daher copiar e colar ele sabe onde
*/
if(instr->op->opds[0].type == LABEL) {
	instr->op->opds[0].type = operand_create_label(opd_read[0]);

 } else if(instr->op->opds[0].type == REGISTER) {
	instr->op->opds[0].type = operand_create_register(atoi(opd_read[0]));

 } else if(instr->op->opds[0].type == STRING) {
	instr->op->opds[0].type = operand_create_string(opd_read[0]);

 } else if(instr->op->opds[0].type == NUMBER_TYPE) {
	instr->op->opds[0].type = operand_create_number(atoi(opd_read[0]));

 }

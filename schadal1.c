// cao p1 at1.cpp : Defines the entry point for the console application.
//1.add,2.sub,3.and,4.or,5.xor,6.movc,7.load,8.store,9.bz,10.bnz,11.jump,12.bal,13.halt,14.nop
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
FILE *fp;
struct list
{
	char instr[100];
	int pos;
	struct list *next, *prev;
};
struct reg_to_value
{
	char reg[3];
	int value_in;
};
struct decoded_instructions
{
	int pc;
	int drnum;
	int ins_type;
	int dest_val;
	int src1_val;
	int src2_val;
	struct decoded_instructions* next;
	struct decoded_instructions* prev;
};
int r[32] = { '\0' };
int X = '\0';
int valid[32] = { 1 };
int address[1000] = { '\0' };
int valid_address[1000] = { 1 };
int num = 20000;
struct decoded_instructions *d_ins_head = NULL, *d_ins_tail = NULL, *d_ins_curr;
static struct list *head = NULL, *curr = NULL, *temp, *fcurr, *dcurr, *ecurr, *mcurr, *wcurr;
int address_transfer = 0;

void load()
{
	char string[100];
	while (fgets(string, 100, (FILE*)fp))
	{
		if (!head)
		{
			temp = malloc(sizeof(struct list));
			temp->next = NULL;
			temp->prev = NULL;
			strcpy(temp->instr, string);
			num++;
			temp->pos = num;
			curr = temp;
			head = curr;
		}
		else
		{
			temp = malloc(sizeof(struct list));
			temp->next = NULL;
			temp->prev = curr;
			num++;
			temp->pos = num;
			strcpy(temp->instr, string);
			curr->next = temp;
			curr = curr->next;
		}
	}
}
void vdisplay()
{
	temp = head;
	while (temp != NULL)
	{
		printf("instr %d %s\n", temp->pos, temp->instr);
		temp = temp->next;
	}

}
int fetch(struct list  *fetchptr)
{
	if (fetchptr)
	{
		if (!dcurr)
		{
			dcurr = fcurr;
			//fcurr = NULL;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
struct decoded_instructions* search_curr_decoded_ins(int pos)
{
	struct decoded_instructions* temp;
	temp = d_ins_tail;
	while (temp)
	{
		if (temp->pc == pos)
		{
			return temp;
		}
		else
		{
			temp = temp->prev;
		}
	}
}
int check_type_of_ins(struct list *raw)
{
	//1.add,2.sub,3.and,4.or,5.xor,6.movc,7.load,8.store,9.bz,10.bnz,11.jump,12.bal,13.halt,14.nop15.mul
	int type = 0;
	const char space[5] = " ,#\t";
	char* ins;// = (char*)malloc(sizeof(char) * 5);

	char *dcurr_str = (char*)malloc(sizeof(char) * 10);
	strcpy(dcurr_str, raw->instr);
	ins = strtok(dcurr_str, space);
	if (!strncmp(ins, "MOVC", 4) || !strncmp(ins, "MOV", 3)) { return 6; }
	else if (!strncmp(ins, "ADD", 3)) { return 1; }
	else if (!strncmp(ins, "SUB", 3)) { return 2; }
	else if (!strncmp(ins, "AND", 3)) { return 3; }
	else if (!strncmp(ins, "OR", 2)) { return 4; }
	else if (!strncmp(ins, "XOR", 3)) { return 5; }
	else if (!strncmp(ins, "MUL", 3)) { return 15; }
	else if (!strncmp(ins, "LOAD", 4)) { return 7; }
	else if (!strncmp(ins, "STORE", 5)) { return 8; }
	else if (!strncmp(ins, "BZ", 2)) { return 9; }
	else if (!strncmp(ins, "BNZ", 3)) { return 10; }
	else if (!strncmp(ins, "BAl", 3)) { return 12; }
	else if (!strncmp(ins, "JUMP", 4)) { return 11; }
	else if (!strncmp(ins, "HALT", 4)) { return 13; }
	else  { return 14; }
	//free(dcurr_str);
}

void decode()
{
	int rnumd = '\0', rnums1 = '\0', rnums2 = '\0';
	char *ins, *dreg, *reg1, *reg2;

	const char space[5] = " ,#\t\n";

	if (dcurr)
	{
		if (!ecurr)
		{
			char *dcurr_str = (char*)malloc(sizeof(char) * 10);
			strcpy(dcurr_str, dcurr->instr);
			//check validity
			//ins = (char*)malloc(sizeof(char) * 5);
			ins = strtok(dcurr_str, space);
			//Normal instructions :destination register is assumed as R register, Source 1 is assumed as register , Source 2 is assumed as either a Register or a literal
			if (!strncmp(ins, "MOVC", 4))
			{
				//dreg = (char*)malloc(sizeof(char) * 3);
				//reg1 = (char*)malloc(sizeof(char) * 3);
				dreg = strtok(NULL, space);
				reg1 = strtok(NULL, space);

				if (!dreg[2])
				{
					rnumd = (int)(dreg[1]) - 48;
				}
				else
				{
					rnumd = (int)(dreg[2] - '0');
					rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
				}
				if (valid[rnumd] == 1)
				{
					valid[rnumd] = 0;
					//decode instruction
					int index = dcurr->pos - 20000;
					struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
					temp_ins->ins_type = check_type_of_ins(dcurr);
					temp_ins->drnum = rnumd;
					if (!strncmp(reg1, "X", 1))
					{
						temp_ins->src1_val = X;
					}
					else
					{
						temp_ins->src1_val = atoi(reg1);
					}
					//temp_ins->src2_val = "\0";
					temp_ins->pc = dcurr->pos;
					//temp_ins->dest_val = "\0";
					temp_ins->next = NULL;
					temp_ins->prev = NULL;
					if (!d_ins_head)
					{
						d_ins_head = temp_ins;
						d_ins_tail = temp_ins;
						d_ins_curr = d_ins_head;
					}
					else
					{
						d_ins_curr->next = temp_ins;
						temp_ins->prev = d_ins_curr;
						d_ins_curr = d_ins_curr->next;

					}
					ecurr = dcurr;
					dcurr = NULL;

				}


			}
			else if (!strncmp(ins, "MOV", 3))
			{
				//dreg = (char*)malloc(sizeof(char) * 3);
				//reg1 = (char*)malloc(sizeof(char) * 3);
				dreg = strtok(NULL, space);
				reg1 = strtok(NULL, space);

				if (!dreg[2])
				{
					rnumd = (int)(dreg[1]) - 48;
				}
				else
				{
					rnumd = (int)(dreg[2] - '0');
					rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
				}
				if (valid[rnumd] == 1)
				{
					valid[rnumd] = 0;
					//decode instruction
					int index = dcurr->pos - 20000;
					struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
					temp_ins->ins_type = check_type_of_ins(dcurr);
					temp_ins->drnum = rnumd;
					if (strncmp(reg1, "X", 1))
					{
						temp_ins->src1_val = X;
					}
					else
					{
						temp_ins->src1_val = atoi(reg1);
					}
					//		temp_ins->src2_val = "\0";
					temp_ins->pc = dcurr->pos;
					//		temp_ins->dest_val = "\0";
					temp_ins->next = NULL;
					temp_ins->prev = NULL;
					if (!d_ins_head)
					{
						d_ins_head = temp_ins;
						d_ins_tail = temp_ins;
						d_ins_curr = d_ins_head;
					}
					else
					{
						d_ins_curr->next = temp_ins;
						temp_ins->prev = d_ins_curr;
						d_ins_curr = d_ins_curr->next;

					}
					ecurr = dcurr;
					dcurr = NULL;

				}


			}
			else if (!strncmp(ins, "ADD", 3) || !strncmp(ins, "SUB", 3) || !strncmp(ins, "AND", 3) || !strncmp(ins, "OR", 2) || !strncmp(ins, "XOR", 3) || !strncmp(ins, "MUL", 3))
			{
				//dreg = (char*)malloc(sizeof(char) * 3);
				//reg1 = (char*)malloc(sizeof(char) * 3);
				dreg = strtok(NULL, space);
				reg1 = strtok(NULL, space);
				if (!reg1[2])
				{
					rnums1 = (int)(reg1[1] - '0');
				}
				else
				{
					rnums1 = (int)(reg1[2] - '0');
					rnums1 = rnums1 + (10 * (int)(reg1[1] - '0'));
				}
				if (valid[rnums1] == 1)
				{
					//reg2 = (char*)malloc(sizeof(char) * 4);
					reg2 = strtok(NULL, space);
					if (!strncmp(reg2, "R", 1))
					{
						if (!reg2[2])
						{
							rnums2 = (int)(reg2[1] - '0');
						}
						else
						{
							rnums2 = (int)(reg2[2] - '0');
							rnums2 = rnums2 + (10 * (int)(reg2[1] - '0'));
						}
						if (valid[rnums2] == 1)
						{
							if (!dreg[2])
							{
								rnumd = (int)(dreg[1] - '0');
							}
							else
							{
								rnumd = (int)(dreg[2] - '0');
								rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
							}
							if (valid[rnumd] == 1)
							{
								valid[rnumd] = 0;
								struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
								temp_ins->ins_type = check_type_of_ins(dcurr);
								temp_ins->drnum = rnumd;

								temp_ins->src1_val = r[rnums1];
								temp_ins->src2_val = r[rnums2];
								temp_ins->pc = dcurr->pos;
								//temp_ins->dest_val = "\0";
								temp_ins->next = NULL;
								d_ins_curr->next = temp_ins;
								temp_ins->prev = d_ins_curr;
								d_ins_curr = d_ins_curr->next;

								ecurr = dcurr;
								dcurr = NULL;
							}
						}
					}
					else
					{

						if (!dreg[2])
						{
							rnumd = (int)(dreg[1] - '0');
						}
						else
						{
							rnumd = (int)(dreg[2] - '0');
							rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
						}
						if (valid[rnumd] == 1)
						{
							valid[rnumd] = 0;
							struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
							temp_ins->ins_type = check_type_of_ins(dcurr);
							temp_ins->drnum = rnumd;

							temp_ins->src1_val = r[rnums1];
							temp_ins->src2_val = atoi(reg2);
							temp_ins->pc = dcurr->pos;
							//temp_ins->dest_val = "\0";
							temp_ins->next = NULL;
							d_ins_curr->next = temp_ins;
							temp_ins->prev = d_ins_curr;
							d_ins_curr = d_ins_curr->next;
							ecurr = dcurr;
							dcurr = NULL;
						}
					}
					//free(reg2);
				}
				//free(reg1);
				//free(dreg);

			}
			else if (!strncmp(ins, "LOAD", 4))
			{
				//dreg = (char*)malloc(sizeof(char) * 3);
				//reg1 = (char*)malloc(sizeof(char) * 3);
				dreg = strtok(NULL, space);
				reg1 = strtok(NULL, space);
				if (!reg1[2])
				{
					rnums1 = (int)(reg1[1] - '0');
				}
				else
				{
					rnums1 = (int)(reg1[2] - '0');
					rnums1 = rnums1 + (10 * (int)(reg1[1] - '0'));
				}
				if (valid[rnums1] == 1)
				{

					//reg2 = (char*)malloc(sizeof(char) * 5);
					reg2 = strtok(NULL, space);
					if (!dreg[2])
					{
						rnumd = (int)(dreg[1] - '0');
					}
					else
					{
						rnumd = (int)(dreg[2] - '0');
						rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
					}
					if (valid[rnumd] == 1)
					{
						valid[rnumd] = 0;
						struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
						temp_ins->ins_type = check_type_of_ins(dcurr);
						temp_ins->drnum = rnumd;
						temp_ins->src1_val = r[rnums1];
						temp_ins->src2_val = atoi(reg2);
						temp_ins->pc = dcurr->pos;
						//						temp_ins->dest_val = "\0";
						temp_ins->next = NULL;
						d_ins_curr->next = temp_ins;
						temp_ins->prev = d_ins_curr;
						d_ins_curr = d_ins_curr->next;
						ecurr = dcurr;
						dcurr = NULL;
					}

					//free(reg2);
				}
				//free(dreg);
				//free(reg1);
			}
			else if (!strncmp(ins, "STORE", 5))
			{
				//dreg = (char*)malloc(sizeof(char) * 3);
				//reg1 = (char*)malloc(sizeof(char) * 3);

				dreg = strtok(NULL, space);
				reg1 = strtok(NULL, space);
				if (!dreg[2])
				{
					rnumd = (int)(dreg[1] - '0');
				}
				else
				{
					rnumd = (int)(dreg[2] - '0');
					rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
				}
				if (valid[rnumd] == 1)
				{
					//reg2 = (char*)malloc(sizeof(char) * 5);
					reg2 = strtok(NULL, space);
					if (!reg1[2])
					{
						rnums1 = (int)(reg1[1] - '0');
					}
					else
					{
						rnums1 = (int)(reg1[2] - '0');
						rnums1 = rnums1 + (10 * (int)(reg1[1] - '0'));
					}
					if (valid[rnums1] == 1)
					{
						struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
						temp_ins->ins_type = check_type_of_ins(dcurr);
						temp_ins->drnum = rnumd;

						temp_ins->src1_val = r[rnums1];
						temp_ins->src2_val = atoi(reg2);
						temp_ins->pc = dcurr->pos;
						temp_ins->dest_val = r[rnumd];
						temp_ins->next = NULL;
						d_ins_curr->next = temp_ins;
						temp_ins->prev = d_ins_curr;
						d_ins_curr = d_ins_curr->next;
						ecurr = dcurr;
						dcurr = NULL;
					}
					//free(reg2);
				}
				//free(dreg);
				//free(reg1);
			}
			else if (!strncmp(ins, "HALT", 4))
			{
				struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
				temp_ins->ins_type = check_type_of_ins(dcurr);
				temp_ins->pc = dcurr->pos;
				temp_ins->next = NULL;
				d_ins_curr->next = temp_ins;
				temp_ins->prev = d_ins_curr;
				d_ins_curr = d_ins_curr->next;
				ecurr = dcurr;
				dcurr = NULL;
			}
			else if (!strncmp(ins, "NOP", 4))
			{
				struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
				temp_ins->ins_type = check_type_of_ins(dcurr);
				temp_ins->pc = dcurr->pos;
				temp_ins->next = NULL;
				d_ins_curr->next = temp_ins;
				temp_ins->prev = d_ins_curr;
				d_ins_curr = d_ins_curr->next;
				ecurr = dcurr;
				dcurr = NULL;

			}
			else if (!strncmp(ins, "BAL", 3))
			{
				//1.add,2.sub,3.and,4.or,5.xor,6.movc,7.load,8.store,9.bz,10.bnz,11.jump,12.bal,13.halt,14.nop15.mul
				struct list* ltemp;
				//dreg = (char*)malloc(sizeof(char) * 3);
				dreg = strtok(NULL, space);
				X = atoi(dreg);
				ltemp = head;
				while (ltemp != NULL && ltemp->pos != X)
				{
					ltemp = ltemp->next;
				}
				if (ltemp)
				{
					char* ltemp_instr = (char*)malloc(sizeof(char) * 10);
					strcpy(ltemp_instr, ltemp->instr);
					ins = strtok(ltemp_instr, space);
					int new_type = check_type_of_ins(ltemp);
					if (new_type == 6)
					{
						//dreg = (char*)malloc(sizeof(char) * 3);
						//reg1 = (char*)malloc(sizeof(char) * 3);
						dreg = strtok(NULL, space);
						reg1 = strtok(NULL, space);

						if (!dreg[2])
						{
							rnumd = (int)(dreg[1]) - 48;
						}
						else
						{
							rnumd = (int)(dreg[2] - '0');
							rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
						}
						if (valid[rnumd] == 1)
						{
							valid[rnumd] = 0;
							//decode instruction
							int index = dcurr->pos - 20000;
							struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
							temp_ins->ins_type = check_type_of_ins(ltemp);
							temp_ins->drnum = rnumd;
							if (strncmp(reg1, "X", 1))
							{
								temp_ins->src1_val = X;
							}
							else
							{
								temp_ins->src1_val = atoi(reg1);
							}
							//temp_ins->src2_val = "\0";
							temp_ins->pc = dcurr->pos;
							//temp_ins->dest_val = "\0";
							temp_ins->next = NULL;
							temp_ins->prev = NULL;
							if (!d_ins_head)
							{
								d_ins_head = temp_ins;
								d_ins_tail = temp_ins;
								d_ins_curr = d_ins_head;
							}
							else
							{
								d_ins_curr->next = temp_ins;
								temp_ins->prev = d_ins_curr;
								d_ins_curr = d_ins_curr->next;

							}
							ecurr = dcurr;
							dcurr = NULL;
						}
					}
					else if ((new_type >= 1 && new_type <= 5) || new_type == 15)
					{
						//dreg = (char*)malloc(sizeof(char) * 3);
						//reg1 = (char*)malloc(sizeof(char) * 3);
						dreg = strtok(NULL, space);
						reg1 = strtok(NULL, space);
						if (!reg1[2])
						{
							rnums1 = (int)(reg1[1] - '0');
						}
						else
						{
							rnums1 = (int)(reg1[2] - '0');
							rnums1 = rnums1 + (10 * (int)(reg1[1] - '0'));
						}
						if (valid[rnums1] == 1)
						{
							//reg2 = (char*)malloc(sizeof(char) * 4);
							reg2 = strtok(NULL, space);
							if (!strncmp(reg2, "R", 1))
							{
								if (!reg2[2])
								{
									rnums2 = (int)(reg2[1] - '0');
								}
								else
								{
									rnums2 = (int)(reg2[2] - '0');
									rnums2 = rnums2 + (10 * (int)(reg2[1] - '0'));
								}
								if (valid[rnums2] == 1)
								{
									if (!dreg[2])
									{
										rnumd = (int)(dreg[1] - '0');
									}
									else
									{
										rnumd = (int)(dreg[2] - '0');
										rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
									}
									if (valid[rnumd] == 1)
									{
										valid[rnumd] = 0;
										struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
										temp_ins->ins_type = check_type_of_ins(ltemp);
										temp_ins->drnum = rnumd;

										temp_ins->src1_val = r[rnums1];
										temp_ins->src2_val = r[rnums2];
										temp_ins->pc = dcurr->pos;
										//temp_ins->dest_val = "\0";
										temp_ins->next = NULL;
										d_ins_curr->next = temp_ins;
										temp_ins->prev = d_ins_curr;
										d_ins_curr = d_ins_curr->next;

										ecurr = dcurr;
										dcurr = NULL;
									}
								}
							}
							else
							{

								if (!dreg[2])
								{
									rnumd = (int)(dreg[1] - '0');
								}
								else
								{
									rnumd = (int)(dreg[2] - '0');
									rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
								}
								if (valid[rnumd] == 1)
								{
									valid[rnumd] = 0;
									struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
									temp_ins->ins_type = check_type_of_ins(dcurr);
									temp_ins->drnum = rnumd;

									temp_ins->src1_val = r[rnums1];
									temp_ins->src2_val = atoi(reg2);
									temp_ins->pc = dcurr->pos;
									//temp_ins->dest_val = "\0";
									temp_ins->next = NULL;
									d_ins_curr->next = temp_ins;
									temp_ins->prev = d_ins_curr;
									d_ins_curr = d_ins_curr->next;
									ecurr = dcurr;
									dcurr = NULL;
								}
							}
							//free(reg2);
						}
						//free(reg1);
						//free(dreg);

					}
					else if (new_type == 7)
					{
						//dreg = (char*)malloc(sizeof(char) * 3);
						//reg1 = (char*)malloc(sizeof(char) * 3);
						dreg = strtok(NULL, space);
						reg1 = strtok(NULL, space);
						if (!reg1[2])
						{
							rnums1 = (int)(reg1[1] - '0');
						}
						else
						{
							rnums1 = (int)(reg1[2] - '0');
							rnums1 = rnums1 + (10 * (int)(reg1[1] - '0'));
						}
						if (valid[rnums1] == 1)
						{

							//reg2 = (char*)malloc(sizeof(char) * 5);
							reg2 = strtok(NULL, space);
							if (!dreg[2])
							{
								rnumd = (int)(dreg[1] - '0');
							}
							else
							{
								rnumd = (int)(dreg[2] - '0');
								rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
							}
							if (valid[rnumd] == 1)
							{
								valid[rnumd] = 0;
								struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
								temp_ins->ins_type = check_type_of_ins(dcurr);
								temp_ins->drnum = rnumd;
								temp_ins->src1_val = r[rnums1];
								temp_ins->src2_val = atoi(reg2);
								temp_ins->pc = dcurr->pos;
								//temp_ins->dest_val = "\0";
								temp_ins->next = NULL;
								d_ins_curr->next = temp_ins;
								temp_ins->prev = d_ins_curr;
								d_ins_curr = d_ins_curr->next;
								ecurr = dcurr;
								dcurr = NULL;
							}

							//free(reg2);
						}
						//free(dreg);
						//free(reg1);

					}
					else if (new_type == 8)
					{
						dreg = strtok(NULL, space);
						reg1 = strtok(NULL, space);
						if (!dreg[2])
						{
							rnumd = (int)(dreg[1] - '0');
						}
						else
						{
							rnumd = (int)(dreg[2] - '0');
							rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
						}
						if (valid[rnumd] == 1)
						{
							//reg2 = (char*)malloc(sizeof(char) * 5);
							reg2 = strtok(NULL, space);
							if (!reg1[2])
							{
								rnums1 = (int)(reg1[1] - '0');
							}
							else
							{
								rnums1 = (int)(reg1[2] - '0');
								rnums1 = rnums1 + (10 * (int)(reg1[1] - '0'));
							}
							if (valid[rnums1] == 1)
							{
								struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
								temp_ins->ins_type = check_type_of_ins(dcurr);
								temp_ins->drnum = rnumd;

								temp_ins->src1_val = r[rnums1];
								temp_ins->src2_val = atoi(reg2);
								temp_ins->pc = dcurr->pos;
								temp_ins->dest_val = r[rnumd];
								temp_ins->next = NULL;
								d_ins_curr->next = temp_ins;
								temp_ins->prev = d_ins_curr;
								d_ins_curr = d_ins_curr->next;
								ecurr = dcurr;
								dcurr = NULL;
							}
							//free(reg2);
						}
						//free(dreg);
						//free(reg1);

					}
					else if (new_type == 13 || new_type == 14)
					{
						struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
						temp_ins->ins_type = check_type_of_ins(dcurr);
						temp_ins->next = NULL;
						d_ins_curr->next = temp_ins;
						d_ins_curr = d_ins_curr->next;
						ecurr = dcurr;
						dcurr = NULL;

					}
					else if (new_type == 9)//|| new_type == 10 || new_type == 11)
					{
						dreg = strtok(NULL, space);
						struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
						temp_ins->ins_type = check_type_of_ins(dcurr);
						temp_ins->dest_val = atoi(dreg);
						temp_ins->pc = dcurr->pos;
						temp_ins->next = NULL;
						d_ins_curr->next = temp_ins;
						temp_ins->prev = d_ins_curr;
						d_ins_curr = d_ins_curr->next;
						ecurr = dcurr;
						dcurr = NULL;

					}
					else if (new_type == 10)
					{
						dreg = strtok(NULL, space);
						struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
						temp_ins->ins_type = check_type_of_ins(dcurr);
						temp_ins->dest_val = atoi(dreg);
						temp_ins->pc = dcurr->pos;
						temp_ins->next = NULL;
						d_ins_curr->next = temp_ins;
						temp_ins->prev = d_ins_curr;
						d_ins_curr = d_ins_curr->next;
						ecurr = dcurr;
						dcurr = NULL;
					}
					else if (new_type == 12)
					{
						printf("\nBAL after BAL exception\n");
					}
					else
					{
						dreg = strtok(NULL, space);
						//reg1 = (char*)malloc(sizeof(char) * 3);
						dreg = strtok(NULL, space);
						reg1 = strtok(NULL, space);
						if (!dreg[2])
						{
							rnumd = (int)(reg1[1] - '0');
						}
						else
						{
							rnumd = (int)(reg1[2] - '0');
							rnumd = rnumd + (10 * (int)(reg1[1] - '0'));
						}
						if (valid[rnumd] == 1)
						{
							valid[rnumd] = 0;
							//decode instruction
							int index = dcurr->pos - 20000;
							struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
							temp_ins->ins_type = check_type_of_ins(dcurr);
							temp_ins->drnum = rnumd;
							if (strncmp(reg1, "X", 1))
							{
								temp_ins->src1_val = X;
							}
							else
							{
								temp_ins->src1_val = atoi(reg1);
							}
							//							temp_ins->src2_val = "\0";
							temp_ins->pc = dcurr->pos;
							//temp_ins->dest_val = "\0";
							temp_ins->next = NULL;
							if (!d_ins_head)
							{
								d_ins_head = temp_ins;
								d_ins_curr = d_ins_head;
							}
							else
							{
								d_ins_curr->next = temp_ins;
								d_ins_curr = d_ins_curr->next;

							}
							ecurr = dcurr;
							dcurr = NULL;

						}

					}
				}

			}




			else if (!strncmp(ins, "JUMP", 4))
			{
				dreg = strtok(NULL, space);
				if (!strncmp(dreg, "R", 1))
				{
					if (!dreg[2])
					{
						rnumd = (int)(dreg[1] - '0');
					}
					else
					{
						rnumd = (int)(dreg[2] - '0');
						rnumd = rnumd + (10 * (int)(dreg[1] - '0'));
					}
					if (valid[rnumd] == 1)
					{
						reg1 = strtok(NULL, space);
						struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
						temp_ins->ins_type = check_type_of_ins(dcurr);
						temp_ins->dest_val = r[rnumd] + atoi(reg1);
						temp_ins->pc = dcurr->pos;
						temp_ins->next = NULL;
						d_ins_curr->next = temp_ins;
						temp_ins->prev = d_ins_curr;
						d_ins_curr = d_ins_curr->next;
						ecurr = dcurr;
						dcurr = NULL;
					}

				}
				else
				{
					struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
					temp_ins->ins_type = check_type_of_ins(dcurr);
					temp_ins->dest_val = atoi(dreg);
					temp_ins->pc = dcurr->pos;
					temp_ins->next = NULL;
					d_ins_curr->next = temp_ins;
					temp_ins->prev = d_ins_curr;
					d_ins_curr = d_ins_curr->next;
					ecurr = dcurr;
					dcurr = NULL;
				}

			}
			else if (!strncmp(ins, "BZ", 2))
			{
				dreg = strtok(NULL, space);
				struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
				temp_ins->ins_type = check_type_of_ins(dcurr);
				temp_ins->dest_val = atoi(dreg);
				temp_ins->pc = dcurr->pos;
				temp_ins->next = NULL;
				d_ins_curr->next = temp_ins;
				temp_ins->prev = d_ins_curr;
				d_ins_curr = d_ins_curr->next;
				ecurr = dcurr;
				dcurr = NULL;
			}
			else if (!strncmp(ins, "BNZ", 3))
			{
				dreg = strtok(NULL, space);
				struct decoded_instructions* temp_ins = (struct decoded_instructions*)malloc(sizeof(struct decoded_instructions));
				temp_ins->ins_type = check_type_of_ins(dcurr);
				temp_ins->dest_val = atoi(dreg);
				temp_ins->pc = dcurr->pos;
				temp_ins->next = NULL;
				d_ins_curr->next = temp_ins;
				temp_ins->prev = d_ins_curr;
				d_ins_curr = d_ins_curr->next;
				ecurr = dcurr;
				dcurr = NULL;
			}

			else
			{
				printf("Wrong instruction encountered %s", ins);
			}
			//free(ins);
			//	address_transfer = (int)dcurr;

			d_ins_tail = d_ins_curr;

		}
	}
}
void execute_ins(struct decoded_instructions* exec_ins)
{
	//1.add,2.sub,3.and,4.or,5.xor,6.movc,7.load,8.store,9.bz,10.bnz,11.jump,12.bal,13.halt,14.nop,15.mul
	struct decoded_instructions* temp_check;
	struct list* next_fetch = head;
	int type = exec_ins->ins_type;
	switch (type)
	{
	case 1:
		//add
		exec_ins->dest_val = exec_ins->src1_val + exec_ins->src2_val;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 2:
		exec_ins->dest_val = exec_ins->src1_val - exec_ins->src2_val;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 3:
		exec_ins->dest_val = exec_ins->src1_val & exec_ins->src2_val;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 4:
		exec_ins->dest_val = exec_ins->src1_val | exec_ins->src2_val;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 5:
		exec_ins->dest_val = exec_ins->src1_val ^ exec_ins->src2_val;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 6:
		exec_ins->dest_val = exec_ins->src1_val;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 7:
		exec_ins->dest_val = exec_ins->src1_val + exec_ins->src2_val;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 8:
		exec_ins->dest_val = exec_ins->src1_val + exec_ins->src2_val;
		valid_address[exec_ins->dest_val] = 0;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 9:

		temp_check = d_ins_head;
		while (temp_check->pc != (ecurr->pos - 1))
		{
			temp_check = temp_check->next;
		}
		if (valid[temp_check->drnum] == 1)
		{
			if (r[temp_check->drnum] == 0)
			{
				int pc_add = exec_ins->dest_val;
				pc_add = ecurr->pos + pc_add;
				struct list* next_fetch = head;
				while (next_fetch->pos != pc_add)
				{
					next_fetch = next_fetch->next;
				}
				fcurr = next_fetch;
				dcurr = NULL;
				ecurr = NULL;
			}
			else
			{
				ecurr = NULL;
			}
		}
		break;
	case 10:
		temp_check = d_ins_head;
		while (temp_check->pc != (ecurr->pos - 1))
		{
			temp_check = temp_check->next;
		}
		if (valid[temp_check->drnum] == 1)
		{
			if (r[temp_check->drnum] != 0)
			{
				int pc_add = exec_ins->dest_val;
				pc_add = ecurr->pos + pc_add;
				struct list* next_fetch = head;
				while (next_fetch->pos != pc_add)
				{
					next_fetch = next_fetch->next;
				}
				fcurr = next_fetch;
				dcurr = NULL;
				ecurr = NULL;
			}
			else
			{
				ecurr = NULL;
			}
		}
		break;
	case 11:
		next_fetch = head;
		while (next_fetch->pos != exec_ins->pc)
		{
			next_fetch = next_fetch->next;
		}
		fcurr = next_fetch;
		dcurr = NULL;
		ecurr = NULL;

		break;
	case 12:
		mcurr = ecurr;
		ecurr = NULL;
		break;
	case 13:
		if (!mcurr)
		{
			if (!wcurr)
			{
				mcurr = ecurr;
				ecurr = NULL;
			}
		}
		break;
	case 14:

		ecurr = NULL;
		break;
	case 15:
		exec_ins->dest_val = exec_ins->src1_val * exec_ins->src2_val;
		mcurr = ecurr;
		ecurr = NULL;
		break;
	default:
		break;
	}
}
void exec()
{


	const char space[5] = " ,#\t";
	//ecurr = (struct list*)address_transfer;
	if (ecurr)
	{
		if (!mcurr)
		{
			if (ecurr->pos == d_ins_curr->pc)
			{
				d_ins_curr->ins_type;
				execute_ins(d_ins_curr);
			}
			else
			{
				struct decoded_instructions* temp_decoded = search_curr_decoded_ins(ecurr->pos);
				execute_ins(temp_decoded);

			}
		}
	}
}
int mem_stage(struct decoded_instructions* temp_ins, struct list* ms_curr)
{
	//1.add,2.sub,3.and,4.or,5.xor,6.movc,7.load,8.store,9.bz,10.bnz,11.jump,12.bal,13.halt,14.nop

	if (temp_ins->ins_type == 7)
	{
		if (valid_address[temp_ins->dest_val] == 1)
		{
			r[temp_ins->drnum] = address[temp_ins->dest_val];
			return 1;
		}
		else
		{
			return 0;
		}

	}
	else if (temp_ins->ins_type == 8)
	{

		address[temp_ins->dest_val] = r[temp_ins->drnum];
		valid_address[temp_ins->dest_val] = 1;
		return 1;
	}
	return 1;
}
void mem()
{
	char *ins;
	const char space[5] = " ,#\t";
	if (mcurr)
	{
		if (!wcurr)
		{
			//ins = (char*)malloc(sizeof(char) * 5);
			char *mcurr_str = (char*)malloc(sizeof(char) * 10);
			strcpy(mcurr_str, mcurr->instr);
			ins = strtok(mcurr_str, space);
			int fs = 0;
			if (!strncmp(ins, "STORE", 5) || !strncmp(ins, "LOAD", 4))
			{
				if (mcurr->pos == d_ins_curr->pc)
				{

					fs = mem_stage(d_ins_curr, mcurr);
				}
				else
				{
					struct decoded_instructions* temp_decoded = search_curr_decoded_ins(mcurr->pos);
					fs = mem_stage(temp_decoded, mcurr);
				}
				if (fs == 1)
				{
					wcurr = mcurr;
					mcurr = NULL;
				}

			}
			//free(ins);
			else
			{
				wcurr = mcurr;
				mcurr = NULL;
			}

			//	free(mcurr_str);
		}
	}
}
void write_back(struct decoded_instructions* temp_decoded)
{
	if ((temp_decoded->ins_type != 7 )&& (temp_decoded->ins_type != 8 )&& (temp_decoded->ins_type != 13))
	{
		r[temp_decoded->drnum] = temp_decoded->dest_val;
		valid[temp_decoded->drnum] = 1;
	}
	else if (temp_decoded->ins_type == 7)
	{
		valid[temp_decoded->drnum] = 1;
	}
}
void wb()
{
	const char space[5] = " ,#\t";
	if (wcurr)
	{
		if (wcurr->pos == d_ins_curr->pc)
		{
			write_back(d_ins_curr);
		}
		else
		{
			struct decoded_instructions* temp_decoded = search_curr_decoded_ins(wcurr->pos);
			write_back(temp_decoded);
		}
		wcurr = NULL;
	}
}

int main()
{
	char *input, *instr1, *instr2; int i = 1, j;
	//char instr1[40], instr2[40];

	while (i != 0)
	{
		printf("\nPlease give the input from the following commands:\n\tload\n\tinitialize\n\tsimulate\n\tdisplay\n\tquit\n");
		input = (char*)malloc(sizeof(char) * 40);
		fgets(input, 40, stdin);
		if (!strncmp(input, "quit", 4))
		{
			i = 0;

		}
		else if (!strncmp(input, "load", 4))
		{
			printf("\n In load\n");
			//instr1 = (char*)malloc(40 * sizeof(char));
			//instr2 = (char*)malloc(40 * sizeof(char));

			strcat(input, " ");
			const char space[2] = " ";
			instr1 = strtok(input, space);
			instr2 = strtok(NULL, space);
			//free(instr2);
			if (instr2 != NULL)
			{

				instr2[strlen(instr2) - 1] = '\0';

				printf("\n|%s|\n", instr2);
				fp = fopen(instr2, "r");
				load();
				printf("load done\n");
				vdisplay();
				printf("\nAll Instructions Loaded done\n");
				fclose(fp);
			}
			else
			{
				printf("\nPlease enter the command in the format load <filename>");
			}
			//instr2[strlen(instr2)] = '\n';
			//free(instr1);
		}
		else if (!strncmp(input, "initialize", 10))
		{

			fcurr = head;
			dcurr = NULL;
			ecurr = NULL;
			mcurr = NULL;
			wcurr = NULL;
			d_ins_curr = NULL;
			d_ins_head = NULL;
			d_ins_tail = NULL;
			X = '\0';
			for (i = 0; i < 32; i++)
			{
				r[i] = 0;
				valid[i] = 1;
			}
			for (i = 0; i < 1000; i++)
			{
				valid_address[i] = 1;
				address[i] = '\0';
			}
			printf("\nInitialized\n");
		}
		else if (!strncmp(input, "simulate", 8))
		{
			int n, status;
			printf("\nIn simulate\n");
			//instr1 = (char*)calloc(40, sizeof(char));
			//instr2 = (char*)calloc(40, sizeof(char));
			strcat(input, " ");
			const char space[2] = " ";
			instr1 = strtok(input, space);
			instr2 = strtok(NULL, space);
			if (instr2 != NULL)
			{
				instr2[strlen(instr2) - 1] = '\0';
				n = atoi(instr2);
				printf("\nRunning for |%s| of |%d| Cycles\n", instr2, n);
				for (j = 1; j <= n; j++)

				{


					wb();
					mem();
					exec();

					decode();
					status = fetch(fcurr);
					if (status == 1)
					{
						fcurr = fcurr->next;
					}


				}

			}
			else { printf("\nPlease enter the command in the format simulate <number of cycles>"); }

		}
		else if (!strncmp(input, "display", 7))
		{
			printf("\nRegister Values \n");
			int i;
			printf("\n***********************************************************************************************************************************\n");
			for (i = 0; i < 32; i++)
			{
				if (!(i % 8))
				{
					printf("\n");
				}
				printf("R[%02d]=%02d ", i, r[i]);
			}
			printf("\n***********************************************************************************************************************************\n");
			printf("\nValues in each stage currently:\n");
			printf("\nInstruction Fetch : %s", fcurr->instr);
			printf("\nInstruction Decode : %s", dcurr->instr);
			printf("\nInstruction Execute : %s", ecurr->instr);
			printf("\nInstruction Memory : %s", mcurr->instr);
			printf("\nInstruction Write Back : %s", wcurr->instr);
			printf("\n***********************************************************************************************************************************\n");
			printf("Memory values:\n");
			for (i = 0; i < 100; i++)
			{
				if (!(i % 10))
				{
					printf("\n");
				}
				printf("Mem[%02d]=%02d ", i, address[i]);
			}
			printf("\n***********************************************************************************************************************************\n");
		}
		free(input);


	}
	return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
    unsigned locx;
    unsigned locy;
    int infected;
    int dead;
} person_t;

typedef struct {
    unsigned pos;
    unsigned total;
} samp_ratio_t;

typedef struct {
    unsigned x_size;
    unsigned y_size;
    samp_ratio_t infect;
    samp_ratio_t recover;
    samp_ratio_t die;
} transmit_config_t;


void transmit_config_set_ratios(transmit_config_t* cfg, 
                                unsigned inf_pos,
                                unsigned inf_total,                                
                                unsigned recover_pos,
                                unsigned recover_total,                                
                                unsigned die_pos,
                                unsigned die_total)
                                {
                                    cfg->infect.pos = inf_pos;
                                    cfg->infect.total = inf_total;
                                    cfg->recover.pos = recover_pos;
                                    cfg->recover.total = recover_total;
                                    cfg->die.pos = die_pos;
                                    cfg->die.total = die_total;
                                }
                                
void transmit_config_set_max(transmit_config_t* cfg, unsigned x_size, unsigned y_size)
{
    cfg->x_size = x_size;
    cfg->y_size = y_size;
}

person_t* person_make(size_t amount)
{
    person_t* people = calloc(1, sizeof(person_t) * amount);
    assert(people != NULL);
    return people;
}



int main(int argc, char const* argv[])
{
    return 0;
}
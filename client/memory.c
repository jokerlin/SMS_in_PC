#include"memory.h"

struct person * new_person()
{
    struct person *ans=(struct person *)MemPoint;
    MemPoint+=sizeof(struct person);
    DataHead[0]+=sizeof(struct person);
    return ans;
}
struct message * new_message()
{
    struct message *ans=(struct message *)MemPoint;
    MemPoint+=sizeof(struct message);
    DataHead[0]+=sizeof(struct message);
    return ans;
}

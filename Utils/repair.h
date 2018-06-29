// 作者: 邓智豪
#ifndef REPAIR_H
#define REPAIR_H
#include "../Algorithm/decode.h"

struct RepairNode;
typedef struct RepairNode RepairNode;
struct RepairNode {
  RepairNode *next;
  int machineIndex;
  int startTime;
  int endTime;
  bool checked;
};

struct Repair;
typedef struct Repair Repair;
struct Repair {
  RepairNode *head;
  RepairNode *tail;
};

void addRepair( int machineIndex, int startTime, int endTime );
Result decodeWithRepair( Gene* gene, DescribeTable table);
void freeReapair();

extern Repair *globalRepairTable;
#endif // REPAIR_H

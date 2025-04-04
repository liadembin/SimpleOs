typedef struct DoubleNode{
  void* value;
  void* metadata;
  struct DoubleNode* next;
  struct DoubleNode* prev;
} DoubleNode;

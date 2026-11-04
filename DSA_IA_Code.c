/* AgriFlow - Smart Irrigation Advisory System for Sugarcane
   Data structures: Queue (array, FIFO) -> Singly Linked List */
#include <stdio.h>
#include <stdlib.h>

#define MAX 10   /* queue size */

/* DEMO thresholds only - configurable, NOT real agricultural standards */
#define MOIST_LOW  30   /* % : below = dry */
#define MOIST_HIGH 60   /* % : above = wet */
#define RAIN_HIGH  10   /* mm: at or above = enough rain */
#define TEMP_HIGH  35   /* C : hot */
#define HUM_LOW    40   /* % : dry air */

/* One sensor record: used as the linked list node and as a queue element */
struct Sensor {
    int id;
    float moisture, temp, humidity, rain;
    struct Sensor *next;
};

struct Sensor *head = NULL;          /* linked list: head = NULL means empty */
struct Sensor queue[MAX];            /* queue: array based, linear */
int front = -1, rear = -1;           /* -1 means queue is empty */

void freeList(void)
{
    struct Sensor *t;
    while (head != NULL) {
        t = head;
        head = head->next;
        free(t);
    }
}

/* Reads a number safely; repeats until it is valid and within min..max */
float getNum(const char *msg, float min, float max)
{
    char buf[100], extra;
    float v;
    while (1) {
        printf("%s", msg);
        if (fgets(buf, sizeof(buf), stdin) == NULL) { freeList(); exit(0); }
        if (sscanf(buf, "%f %c", &v, &extra) != 1)
            printf("Invalid input. Enter a number.\n");
        else if (!(v >= min && v <= max))
            printf("Value must be between %.0f and %.0f.\n", min, max);
        else
            return v;
    }
}

/* ---------------- QUEUE ---------------- */

int isEmpty(void) { return front == -1; }

int enqueue(struct Sensor s)             /* add at REAR */
{
    if (rear == MAX - 1) return 0;       /* overflow */
    if (isEmpty()) front = 0;
    queue[++rear] = s;
    return 1;
}

int dequeue(struct Sensor *s)            /* remove from FRONT */
{
    if (isEmpty()) return 0;
    *s = queue[front];
    if (front == rear) front = rear = -1;   /* queue became empty: reset */
    else front++;
    return 1;
}

int inQueue(int id)
{
    int i;
    if (isEmpty()) return 0;
    for (i = front; i <= rear; i++)
        if (queue[i].id == id) return 1;
    return 0;
}

void printHeader(void)
{
    printf("%-6s %9s %7s %9s %9s\n", "ID", "Moisture", "Temp", "Humidity", "Rain");
}

void printRow(struct Sensor *p)
{
    printf("%-6d %9.1f %7.1f %9.1f %9.1f\n",
           p->id, p->moisture, p->temp, p->humidity, p->rain);
}

void showQueue(void)
{
    int i;
    if (isEmpty()) { printf("No pending readings. Queue is empty.\n"); return; }
    printHeader();
    for (i = front; i <= rear; i++)
        printRow(&queue[i]);
    printf("Next to be processed: Sensor %d (front)\n", queue[front].id);
}

/* ---------------- LINKED LIST ---------------- */

struct Sensor *search(int id)            /* returns node or NULL */
{
    struct Sensor *t;
    for (t = head; t != NULL; t = t->next)
        if (t->id == id) return t;
    return NULL;
}

void insertEnd(struct Sensor s)          /* add a new node at the end */
{
    struct Sensor *t, *n = (struct Sensor *)malloc(sizeof(struct Sensor));
    if (n == NULL) { printf("Memory allocation failed.\n"); return; }
    *n = s;
    n->next = NULL;
    if (head == NULL) head = n;
    else {
        t = head;
        while (t->next != NULL) t = t->next;
        t->next = n;
    }
}

void deleteRecord(int id)
{
    struct Sensor *cur = head, *prev = NULL;
    if (head == NULL) { printf("No records stored. Nothing to delete.\n"); return; }
    while (cur != NULL && cur->id != id) {   /* find the node */
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) { printf("Sensor ID %d not found.\n", id); return; }
    if (prev == NULL) head = cur->next;      /* first node */
    else prev->next = cur->next;             /* middle or last node */
    free(cur);
    printf("Record deleted.\n");
}

void showList(void)
{
    struct Sensor *t;
    if (head == NULL) { printf("No stored records.\n"); return; }
    printHeader();
    for (t = head; t != NULL; t = t->next)
        printRow(t);
}

int countRecords(void)
{
    int c = 0;
    struct Sensor *t;
    for (t = head; t != NULL; t = t->next)
        c++;
    return c;
}

/* ---------------- FEATURES ---------------- */

void addReading(void)
{
    struct Sensor s;
    if (rear == MAX - 1) {
        printf("Queue overflow! Process data (option 2) first.\n");
        return;
    }
    s.id = (int)getNum("Sensor ID (1-9999): ", 1, 9999);
    if (search(s.id) != NULL || inQueue(s.id)) {
        printf("Sensor ID %d already exists. Duplicate not allowed.\n", s.id);
        return;
    }
    s.moisture = getNum("Soil Moisture (0-100 %): ", 0, 100);
    s.temp     = getNum("Temperature (-10 to 60 C): ", -10, 60);
    s.humidity = getNum("Humidity (0-100 %): ", 0, 100);
    s.rain     = getNum("Rainfall (0-500 mm): ", 0, 500);
    s.next = NULL;
    enqueue(s);
    printf("Reading added to queue.\n");
}

void processData(void)                   /* Queue -> Linked List */
{
    struct Sensor s;
    if (!dequeue(&s)) { printf("No new data. Queue is empty.\n"); return; }
    insertEnd(s);
    printf("Sensor %d processed and stored in the linked list.\n", s.id);
}

/* Simple if-else advice based on the demo thresholds (no AI) */
const char *advice(struct Sensor *p)
{
    if (p->rain >= RAIN_HIGH) return "Irrigation Not Required (enough rainfall)";
    if (p->moisture < MOIST_LOW) return "Irrigation Required (soil is dry)";
    if (p->moisture > MOIST_HIGH) return "Irrigation Not Required (soil is wet)";
    if (p->temp >= TEMP_HIGH && p->humidity < HUM_LOW)
        return "Irrigation Required (hot and dry weather)";
    return "Monitor Soil Conditions";
}

void showAdvisory(void)
{
    struct Sensor *t;
    if (head == NULL) { printf("No stored records. Process some readings first.\n"); return; }
    printf("(Thresholds are demo values set in the code, not real standards)\n");
    for (t = head; t != NULL; t = t->next)
        printf("Sensor %-5d: %s\n", t->id, advice(t));
}

int main(void)
{
    int choice;
    do {
        printf("\n--- AGRIFLOW: SMART IRRIGATION ADVISORY SYSTEM ---\n"
               "1. Add New Sensor Reading\n"
               "2. Process Incoming Sensor Data\n"
               "3. Display Pending Sensor Readings\n"
               "4. Display All Stored Sensor Records\n"
               "5. Search Sensor Record\n"
               "6. Delete Sensor Record\n"
               "7. Generate Irrigation Advisory\n"
               "8. Display Total Stored Records\n"
               "9. Exit\n");
        choice = (int)getNum("Enter your choice: ", 1, 9);
        switch (choice) {
        case 1: addReading(); break;
        case 2: processData(); break;
        case 3: showQueue(); break;
        case 4: showList(); break;
        case 5:
            if (head == NULL) printf("No stored records.\n");
            else {
                struct Sensor *f = search((int)getNum("Sensor ID to search: ", 1, 9999));
                if (f == NULL) printf("Sensor not found.\n");
                else { printHeader(); printRow(f); }
            }
            break;
        case 6:
            if (head == NULL) printf("No stored records. Nothing to delete.\n");
            else deleteRecord((int)getNum("Sensor ID to delete: ", 1, 9999));
            break;
        case 7: showAdvisory(); break;
        case 8: printf("Total stored records: %d\n", countRecords()); break;
        case 9: freeList(); printf("Memory freed. Goodbye!\n"); break;
        }
    } while (choice != 9);
    return 0;
}

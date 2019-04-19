#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>

struct SLnode{
    int key;
    SLnode* *next;
};

struct skiplist{
    SLnode* first;
    SLnode* last;
    int max_lvl;
};

SLnode* create_slnode(int lvl, int key){
    SLnode* tmp = new SLnode;
    tmp->next = new SLnode*[lvl];
    for(int i = 0; i < lvl; i++){
        tmp->next[i] = new SLnode;
        tmp->next[i]->key = key;
    }
    tmp->key = key;
    return tmp;
}

skiplist* create(int max_value){
    skiplist* tmp = new skiplist;
    tmp->max_lvl = 4;
    tmp->first = create_slnode(tmp->max_lvl, 0);
    tmp->last = create_slnode(tmp->max_lvl, max_value);
    tmp->last->next = NULL;
    for(int i = 0; i < tmp->max_lvl; i++){
        tmp->first->next[i] = tmp->last;
    }
    return tmp;
}

SLnode* find(skiplist* SL, int key){
    SLnode* p = SL->first;
    for(int lvl = SL->max_lvl-1; lvl >= 0; lvl--){
        while(key > p->next[lvl]->key) p = p->next[lvl];
    }
    p = p->next[0];
    if(p->key == key) return p;
    return NULL;
}

int get_level(skiplist* SL){
    int lvl = 1;
    /*time_t tt;
    int seed = (int)time(&tt);
    srand(seed);*/
    std::random_device rd;
    srand(rd());
    while(random()%10000 < 5000 and lvl < SL->max_lvl) lvl++;
    return lvl;
}

void insert(skiplist* SL, int search_key){
    SLnode* *update = new SLnode*[SL->max_lvl];
    for(int i = 0; i < SL->max_lvl; i++){
        update[i] = new SLnode;
    }
    SLnode* tmp = SL->first;
    for(int i = SL->max_lvl-1; i >=0; i--){
        while(tmp->next[i]->key < search_key) tmp = tmp->next[i];
        update[i] = tmp;
        //printf("key: %d level: %d\n", tmp->key, i);
    }
    tmp = tmp->next[0];
    if(tmp->key == search_key) tmp->key = search_key; // this if-else is needed when you have key and value(you can update value in existing key then)
    else{
        int new_lvl = get_level(SL);
        /* case: new_lvl > SL->max_lvl - using update[] vector */
        tmp = create_slnode(new_lvl, search_key);
        //printf("newlvl: %d\n", new_lvl);
        for(int i = 0; i < new_lvl; i++) { // SL->max_lvl
            tmp->next[i] = update[i]->next[i];
            update[i]->next[i] = tmp;
        }
    }
}

void pop(skiplist* SL, int search_key){
    SLnode* *update = new SLnode*[SL->max_lvl];
    for(int i = 0; i < SL->max_lvl; i++){
        update[i] = new SLnode;
    }
    SLnode* tmp = SL->first;
    for(int i = SL->max_lvl-1; i >=0; i--){
        while(tmp->next[i]->key < search_key) tmp = tmp->next[i];
        update[i] = tmp;
        //printf("key: %d level: %d\n", tmp->key, i);
    }
    tmp = tmp->next[0];
    if(tmp->key == search_key){
        for(int i = 0; i < SL->max_lvl; i++){
            if(update[i]->next[i] != tmp) break;
            update[i]->next[i] = tmp->next[i];
            tmp->next[i] = NULL;
            delete tmp->next[i];
        }
        while(SL->max_lvl > 0 and SL->first->next[SL->max_lvl-1] == SL->last) SL->max_lvl--;
    }
}

void show(skiplist* SL, int max_val){
    SLnode* tmp = SL->first;
    for(int i = SL->max_lvl-1; i >=0; i--){
        tmp = SL->first->next[i];
        printf("level %d:\n", i);
        while(tmp->key < max_val) {
            printf("%d ", tmp->key);
            tmp = tmp->next[i];
        }
        printf("\n");
    }
    printf("maxlvl: %d\n", SL->max_lvl);
}
int main() {
    int max_val = 100;
    skiplist* S = create(max_val);
    int tab[] = {21, 19, 26, 7, 25, 12, 17, 6, 9, 3};
    for(int i = 0; i < 10; i++){
        insert(S, tab[i]);
    }
    show(S, max_val);
    for(int i = 0; i < 10; i++){
        pop(S, tab[i]);
        show(S, max_val);
    }

}
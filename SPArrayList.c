#include <stdio.h>
#include <stdlib.h>
#include "SPArrayList.h"

#include <string.h>


SPArrayList* spArrayListCreate(int maxSize)//V//
{
    SPArrayList* arrayList = (SPArrayList*)calloc(1,sizeof(SPArrayList));
    if (maxSize<=0 || arrayList==NULL)
    {
        return  NULL;
    }
    else
    {
        arrayList->maxSize= maxSize;
        arrayList->actualSize = 0;
        arrayList->elements = (int*)calloc(1,maxSize* sizeof(int));
        return arrayList;
    }

}

SPArrayList* spArrayListCopy(SPArrayList* src)//V//
{
    SPArrayList* newList = (SPArrayList*)malloc(sizeof(SPArrayList));
    if (src!=NULL && newList!=NULL)
    {
        memcpy(newList,src, sizeof(SPArrayList));
        return newList;
    }
    else
    {
        return NULL;
    }
}

void spArrayListDestroy(SPArrayList* src)//V//
{
    if (src!=NULL)
    {
        free(src);
    }
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src)
{
    if (src==NULL)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    else
    {
        for(int i=0;i<src->actualSize;i++){
            src->elements[i]=0;
        }

        src->actualSize = 0;
        return SP_ARRAY_LIST_SUCCESS;
    }
}
SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index)
{
    if (src==NULL || index>src->actualSize)  //is it ok to check bigger then actual?
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    else
    {
        if (src->actualSize == src->maxSize)
        {
            return SP_ARRAY_LIST_FULL;
        }
        else
        {
            for(int i=src->actualSize-1;i==index; i--)
            {
                src->elements[i+1]=src->elements[i];
            }
            src->elements[index] = elem;
            src->actualSize++;
            return SP_ARRAY_LIST_SUCCESS;
        }
    }

}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem)//V//
{
    return spArrayListAddAt(src,elem,0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem)//V//
{
    return spArrayListAddAt(src,elem,src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index)
{
    if (src==NULL || index>=src->actualSize)  //check conditions?
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    else
    {
        if (src->actualSize == 0)
        {
            return SP_ARRAY_LIST_EMPTY;
        }
        else
        {
            for(int i=index; i==src->actualSize-2; i++)
            {
                src->elements[i]=src->elements[i+1];  //HOW TO WRITE END OF ARRAY or delete?? i just changed the size?
            }
            src->actualSize--;
            return SP_ARRAY_LIST_SUCCESS;
        }
    }
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src)//V//
{
    return spArrayListRemoveAt(src,0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src)
{
    return spArrayListRemoveAt(src,src->actualSize-1);
}

int spArrayListGetAt(SPArrayList* src, int index)
{
    if (src==NULL || index>=src->actualSize || index<0)
    {
        return -99;
    }
    else
    {
        return src->elements[index];
    }
}

int spArrayListGetFirst(SPArrayList* src)
{
    if (src==NULL || src->actualSize==0)
    {
        return -99;
    }
    return spArrayListGetAt(src,0);
}

int spArrayListGetLast(SPArrayList* src)//V//
{
    if (src==NULL || src->actualSize==0)
    {
        return -99;
    }

    return spArrayListGetAt(src,src->actualSize-1);
}

int spArrayListMaxCapacity(SPArrayList* src)
{
    if (src == NULL)
    {
        return -99;
    }
    return src->maxSize;
}

int spArrayListSize(SPArrayList* src)//V//
{
    if (src == NULL)
    {
        return -99;
    }
    return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src)//V//
{
    if (src==NULL || src->actualSize<src->maxSize)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool spArrayListIsEmpty(SPArrayList* src)
{
    if (src==NULL || src->actualSize!=0)
    {
        return false;
    }
    else
    {
        return true;
    }
}
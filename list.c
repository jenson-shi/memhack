/*
  Copyright (c) 2014 Jenson Shi <jenson.shixf@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include "head.h"


typedef struct MH_find_t
{
    LPVOID                  address;
    int                     flag;
    struct MH_find_t*		next;
}MH_find_t;


static MH_find_t* head = 0;
static MH_find_t* end = 0;


void MH_AddToList(LPVOID address)
{
    MH_find_t* node;

	node = malloc(sizeof(MH_find_t));
    node->address = address;
    node->flag = 1;
    node->next = 0;

    if(!head)
    {
		head = node;
        end = head;
        return;
    }
    
    end->next = node;
    end = end->next;
}


void MH_SetFlagList(LPVOID address)
{
    MH_find_t* find = head;
    while(find)
    {
        if(address == find->address)
        {
            find->flag = 1;
        }
        find = find->next;
    }
}


void MH_ClearFlagList()
{
    MH_find_t* cur = head;
    while(cur)
    {
        cur->flag = 0;
        cur = cur->next;
    }
}


void MH_UpdateList()
{
    MH_find_t* cur = head;
    MH_find_t* parent = 0;
    MH_find_t* tmp;
    DWORD value;
    DWORD readsize = 0;
    while(cur)
    {
        if(!cur->flag)
        {
            tmp = cur->next;
            free(cur);
            cur = tmp;
            if(parent)
            {
                parent->next = cur;
            }
            else
            {
                head = cur;
            }
        }
        else
        {
            ReadProcessMemory(pm.handle, cur->address, &value, sizeof(value), &readsize);
            printf("0x%0p [%u]\n", cur->address, value);
            parent = cur;
            cur = cur->next;
        }
    }
}


void MH_ClearList()
{
    MH_find_t* tmp;
    while(head)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
}


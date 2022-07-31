#include "memManage.h"

#define SORT_CONDITION(mem1,mem2,tag) (((tag) == B_FIT) ? \
							((mem1)->mem_left_size > (mem2)->mem_left_size) : \
							((mem2)->mem_left_size) > (mem1)->mem_left_size)
// ���� ��ó����� ���� ������ ��ȯ
// best fit�� ��� - (temp_1->mem_left_size > temp_2->mem_left_size)
// worst fit�� ��� - (temp_1->mem_left_size < temp_2->mem_left_size)

void setTag() {
	do {
		cout << "0 : first fit" << endl << "1 : best fit" << endl << "2 : worst fit" << endl << "�Է� >> ";
		cin >> tag;
		if (!((tag == 0) || (tag == 1) || (tag == 2))) {
			system("cls");
			cout << "�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���." << endl;
		}
	} while ((tag != 0) && (tag != 1) && (tag != 2));

	system("PAUSE");
	system("cls");
}

void initMem() {
	available = new Chunk;
	available->link = NULL;
	available->mem_left_size = MAX_MEM; // ���Ŀ� ����ڰ� �ִ� �޸� ũ�� ����
	available->mem_start = 0;
}

void releaseMem() { // ��� �޸� �Ҵ� ����
	Chunk *temp = available;
	Chunk *del = temp;
	do {
		temp = temp->link;
		delete del;
		del = temp;
	} while (temp->link != NULL);
}

int myalloc(int req_size) {
	Chunk *current = available; // ������ ���
	Chunk *temp2 = NULL; // 
	Chunk *del_link = NULL; // ������ ����� ���� ��带 �˻�
	int return_mem_start;
	while (current != NULL) { // �޸� �Ҵ��� ������ �˻�
		if (req_size <= current->mem_left_size) {
			// ���� : ��û�� ũ�Ⱑ ���� ����� ���� �뷮���� ���� ���
			return_mem_start = current->mem_start;
			current->mem_start += req_size;
			current->mem_left_size -= req_size;
			// ���� ����� ���� ����
			printf("mem allocated succeced\t%3d, size %3d\n", current->mem_start - req_size, req_size);
			if (current->mem_left_size == 0) { // ���� �޸� �뷮�� 0�� ��� �޸� �Ҵ����� �ʿ�
				del_link = available; // �������� �˻�
				if (current == available) { // ���� ��尡 ������� ���
					available = current->link;
				}
				else { // ���� ��尡 ��� ��尡 �ƴ� ���
					while (del_link->link != current) { // ������ ����� ���� ��带 �˻�
						del_link = del_link->link;
					}
					del_link->link = current->link;
				}
				delete current;
			}
			// �޸� ���� �ּ� ��ȯ
			return return_mem_start;
			//return (available->mem_start - req_size);
		}
		current = current->link;
	}
	// error : �޸� �Ҵ� ����
	printf("mem allocated falied\t%3d\n", -1);
	return -1;
}

// F_FIT B_FIT W_FIT

void myfree(int start_addr, int return_size) {
	Chunk *temp = available;

	while (temp->link != NULL) temp = temp->link;
	temp->link = new Chunk;
	temp = temp->link;
	setNode(temp, start_addr, return_size);

	switch (tag) {
	case F_FIT:
		memMerge();
		break;
	case B_FIT: // ���� ������ ���� �߻�
	case W_FIT:
		bwfitSort();
		memMerge();
		bwfitSort();
		break;
	}

	printf("mem free\n");
}

void setNode(Chunk *temp, int start_addr, int return_size) { // ��� ����
	temp->link = NULL;
	temp->mem_start = start_addr;
	temp->mem_left_size = return_size;
}

void memMerge() {	// best, worst fit �޸� ����
	Chunk *current;				// ���� ���
	Chunk *temp;				// ã���ִ� ���
	Chunk *head = available;	// �˻��� ������ �Ǵ� ���
	Chunk *mid_node;			// ã���ִ� ����� ������ ����Ű�� ���

	while (head != NULL) {
		current = head; // head�� ���� �ǵ帮�� �ʰڴٴ� ���� ����
		temp = current->link; // ã���ִ� ��� != ���� ���

		while (temp != NULL) {
			if (current->mem_start + current->mem_left_size == temp->mem_start) {
				// ���� ����� �޸� �����ּ� + �޸� ���� �뷮 == ã���ִ� ����� �޸� �����ּ� ? ���� : ���� ���
				current->mem_left_size += temp->mem_left_size; // ���� ����� ���� �޸� �뷮�� ã���ִ� ����� ���� �޸� �뷮�� ����
				mid_node = current;	// ã���ִ� ����� ���� ��带 ã�� ����
				while (mid_node->link != temp) {
					mid_node = mid_node->link;
				}
				mid_node->link = temp->link;	// 1��2��3��4 ��尡 ���� �� temp node = 3�� ���, mid node = 2
												// mid node->link = temp node->link�� ���� 1��2��4�� �����
				delete temp; // ã���ִ� ��� �޸� ����
				temp = current->link;
				continue;	// ã���ִ� ��带 ���� ����� next������ ����, while�� ��� ����
			}
			else if (current->mem_start == temp->mem_start + temp->mem_left_size) {
				// �������� �޸� �����ּ� == ã���ִ� �����; �޸� �����ּ� + ���� �뷮 ? ���� : ���� ���
				current->mem_start = temp->mem_start;
				current->mem_left_size += temp->mem_left_size;
				current->link = temp->link;
				// current ����� �޸� ������ �պ��� �������� ����
				delete temp;
				temp = current->link;
				continue;
			}
			temp = temp->link;
		}
		head = head->link;
	}

}

void bwfitSort() {
	Chunk *temp_1 = available, *temp_2 = NULL;

	for (; temp_1->link != NULL; temp_1 = temp_1->link) {
		for (temp_2 = temp_1->link; temp_2 != NULL; temp_2 = temp_2->link) {
			if (SORT_CONDITION(temp_1, temp_2, tag)) bwfitSwap(temp_1, temp_2);
		}
	}
}

void bwfitSwap(Chunk *temp_1, Chunk *temp_2) { // ������ swap
	int mem_left_size, mem_start;

	mem_start = temp_1->mem_start;
	temp_1->mem_start = temp_2->mem_start;
	temp_2->mem_start = mem_start;

	mem_left_size = temp_1->mem_left_size;
	temp_1->mem_left_size = temp_2->mem_left_size;
	temp_2->mem_left_size = mem_left_size;
}

void bwfitMerge() {
	Chunk *temp = available->link;
	Chunk *left = available;
	while (temp != NULL) {
		while (left->mem_left_size + left->mem_start == temp->mem_start) {
			//bwfitMergeData(left, temp);
			left->mem_left_size += temp->mem_left_size;
			temp = temp->link;
			delete left->link;
			left->link = temp;
		}
		left = left->link;
		temp = temp->link;
	}

}

void printMemStat() {
	Chunk *temp = available;

	for (int cnt = 0; temp != NULL; cnt++) {
		printf("(%3d) mem_start_addr : %3d, mem_left_size : %3d\n", cnt + 1, temp->mem_start, temp->mem_left_size);
		temp = temp->link;
	}
}

void *marked(int *arr, int size);

int main() {
    int mark[5] = {19, 10, 8, 17, 9};
    marked(mark, 5);

    for (int i = 0; i < 5; i++) {
        printf("%d\n", mark[i]);
    }

}

void *marked(int *arr, int size) {

    int count = 0;

    for (int i = 0; i < size; i++, ++count) {
        arr[i] = count;
    }
}
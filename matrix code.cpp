#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>

using namespace std;

class DynamicSquareMatrix {
private:
    int** fullData;
    int** shadedData;
    int Rows;
    int Cols;


    int Random(int Low, int Hi) {
        return Low + rand() % (Hi - Low + 1);
    }

    void allocateMemory() {
        fullData = new int*[Rows];
        for (int i = 0; i < Rows; i++) {
            fullData[i] = new int[Cols];
        }
    }

    void allocateConstraintMemory() {
        shadedData = new int*[Rows];
        int len = -1;
        for (int i = 0; i < Rows; i++) {
            if(i <= Rows/2) { len += 1; }
            if(i > Rows/2) { len -= 1; }
            shadedData[i] = new int[Cols - len];
        }
    }


    void freeFullMemory() {
        for (int i = 0; i < Rows; i++) {
            delete[] fullData[i];
        }
        delete[] fullData;
    }

    void freeShadedMemory() {
        for (int i = 0; i < Rows; i++) {
            delete[] shadedData[i];
        }
        delete[] shadedData;
    }

    int getRowOfMaxInShadedArea() {
        int maxVal = shadedData[0][0];
        int maxRow = 0;
        int len = -1;
        for (int i = 0; i < Rows; i++) {
            if(i <= Rows/2) { len += 1; }
            if(i > Rows/2) { len -= 1; }
            for (int j = 0; j < Cols - len; j++) {
                if (shadedData[i][j] > maxVal) {
                    maxVal = shadedData[i][j];
                    maxRow = i;
                }
            }
        }
        return maxRow;
    }

    void deleteRowFromFullMatrix(int rowToDelete) {
        if (rowToDelete < 0 || rowToDelete >= Rows) return;

        int newRows = Rows - 1;
        int** newFullData = new int*[newRows];

        int newRow = 0;
        for (int i = 0; i < Rows; i++) {
            if (i == rowToDelete) continue;

            newFullData[newRow] = new int[Cols];
            for (int j = 0; j < Cols; j++) {
                newFullData[newRow][j] = fullData[i][j];
            }
            newRow++;
        }

        for (int i = 0; i < Rows; i++) {
            delete[] fullData[i];
        }
        delete[] fullData;

        fullData = newFullData;
        Rows = newRows;
    }

    void rebuildShadedData() {
        for (int i = 0; i < Rows + 1; i++) {
            delete[] shadedData[i];
        }
        delete[] shadedData;
        allocateConstraintMemory();
        transferToShadedMatrix();
    }

public:
    DynamicSquareMatrix(int n) {
        if (n < 11 || n % 2 == 0) {
            cerr << "Ошибка: размер должен быть >= 11 и нечётным. Установлен 11.\n";
            Rows = 11;
            Cols = 11;
        } else {
            Rows = n;
            Cols = n;
        }
        allocateMemory();
        allocateConstraintMemory();
        fillRandom(-10000, 10000);
        transferToShadedMatrix();
    }
    ~DynamicSquareMatrix() {
        freeFullMemory();
        freeShadedMemory();
    }

    void fillRandom(int Low, int Hi) {
        srand(time(NULL));
        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Cols; j++) {
                fullData[i][j] = Random(Low, Hi);
            }
        }
    }

    void transferToShadedMatrix() {
        int len = -1;
        for (int i = 0; i < Rows; i++) {
            if(i <= Rows/2) { len += 1; }
            if(i > Rows/2) { len -= 1; }
            int rowLength = Cols - len;
            for (int j = 0; j < rowLength; j++) {
                shadedData[i][j] = fullData[i][j];
            }
        }
    }

    void printToConsole() {
        int rowToDelete = getRowOfMaxInShadedArea();
        int maxVal = getMax();
        cout << "=== ОРИГИНАЛЬНАЯ МАТРИЦА ===" << endl;
        cout << "Размер матрицы: " << Rows << "x" << Cols << endl;
        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Cols; j++) {
                cout << fullData[i][j] << "\t";
            }
            cout << endl;

        }
        cout << "\nЗаштрихованная область:\n";
        int le1n = -1;
        for (int i = 0; i < Rows; i++) {
            if(i <= Rows/2) { le1n += 1; }
            if(i > Rows/2) { le1n -= 1; }
            for (int j = 0; j < Cols - le1n; j++) {
                cout << shadedData[i][j] << "\t";
            }
            cout << endl;

        }
        cout << "\n=== Результаты задания 2 ===" << endl;
        cout << "2a. Максимум: " << getMax() << endl;
        cout << "2a. Минимум: " << getMin() << endl;
        cout << "2b. Диапазон: " << getRangeInShaded() << endl;
        cout << "2b. Количество: " << countElementsNearMax() << endl;
    }

    void deleteRowWithMaxInShadedArea() {
        int rowToDelete = getRowOfMaxInShadedArea();
        int maxVal = getMax();

        cout << "\n=== ЗАДАНИЕ 3 ===" << endl;
        cout << "Максимальный элемент в заштрихованной области: " << maxVal << endl;
        cout << "Он находится в строке " << rowToDelete + 1 << endl;
        int len = -1;
        for (int i = 0; i <= rowToDelete; i++) {
            if(i <= Rows/2) { len += 1; }
            if(i > Rows/2) { len -= 1; }
        }
        cout << "Содержимое удаляемой строки: ";
        for (int j = 0; j < Cols - len; j++) {
            cout << shadedData[rowToDelete][j] << " ";
        }
        cout << endl;
        deleteRowFromFullMatrix(rowToDelete);
        rebuildShadedData();
        cout << "Новый размер матрицы: " << Rows << "x" << Cols << endl;
        cout << "=== МАТРИЦА ПОСЛЕ УДАЛЕНИЯ ===" << endl;
        cout << "Размер матрицы: " << Rows << "x" << Cols << endl;
        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Cols; j++) {
                cout << fullData[i][j] << "\t";
            }
            cout << endl;
        }
        cout << "\n=== НОВЫЕ ХАРАКТЕРИСТИКИ (после удаления) ===" << endl;
        cout << "Максимум в заштрихованной области: " << getMax() << endl;
        cout << "Минимум в заштрихованной области: " << getMin() << endl;
        cout << "Диапазон (max - min): " << getRangeInShaded() << endl;
    }

    int getMax() {
        int maxVal = shadedData[0][0];
        int len = -1;
        for (int i = 0; i < Rows; i++) {
            if(i <= Rows/2) { len += 1; }
            if(i > Rows/2) { len -= 1; }
            for (int j = 0; j < Cols - len; j++) {
                if (shadedData[i][j] > maxVal) {
                    maxVal = shadedData[i][j];
                }
            }
        }
        return maxVal;
    }

    int getMin() {
        int minVal = shadedData[0][0];
        int len = -1;
        for (int i = 0; i < Rows; i++) {
            if(i <= Rows/2) { len += 1; }
            if(i > Rows/2) { len -= 1; }
            for (int j = 0; j < Cols - len; j++) {
                if (shadedData[i][j] < minVal) {
                    minVal = shadedData[i][j];
                }
            }
        }
        return minVal;
    }

    int getRangeInShaded() {
        return getMax() - getMin();
    }

    int countElementsNearMax() {
        int maxVal = getMax();
        int minVal = getMin();
        int range = maxVal - minVal;
        double threshold = maxVal - 0.2 * range;
        int Count = 0;
        int len = -1;
        for (int i = 0; i < Rows; i++) {
            if(i <= Rows/2) { len += 1; }
            if(i > Rows/2) { len -= 1; }
            for (int j = 0; j < Cols - len; j++) {
                if (shadedData[i][j] > threshold && shadedData[i][j] < maxVal) {
                    Count++;
                }
            }
        }
        return Count;
    }

    void saveOriginalToFile(const char* filename) {
        ofstream out(filename);
        out << "=== ОРИГИНАЛЬНАЯ МАТРИЦА ===" << endl;
        out << "Размер матрицы: " << Rows << "x" << Cols << endl;
        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Cols; j++) {
                out << fullData[i][j] << "\t";
            }
            out << endl;
        }
        out << "\n=== Результаты задания 2 ===" << endl;
        out << "2a. Максимум: " << getMax() << endl;
        out << "2a. Минимум: " << getMin() << endl;
        out << "2b. Диапазон: " << getRangeInShaded() << endl;
        out << "2b. Количество: " << countElementsNearMax() << endl;
        int rowToDelete = getRowOfMaxInShadedArea();
        int maxVal = getMax();
        out << "\n=== ЗАДАНИЕ 3 ===" << endl;
        out << "Максимальный элемент в заштрихованной области: " << maxVal << endl;
        out << "Он находится в строке " << rowToDelete + 1 << endl;
        int len = -1;
        for (int i = 0; i <= rowToDelete; i++) {
            if(i <= Rows/2) { len += 1; }
            if(i > Rows/2) { len -= 1; }
        }
        out << "Содержимое удаляемой строки: ";
        for (int j = 0; j < Cols - len; j++) {
            out << shadedData[rowToDelete][j] << " ";
        }
        out << endl;
        out.close();
    }

    void saveResultToFile(const char* filename) {
        ofstream out(filename);
        out << "=== МАТРИЦА ПОСЛЕ УДАЛЕНИЯ ===" << endl;
        out << "Размер матрицы: " << Rows << "x" << Cols << endl;
        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Cols; j++) {
                out << fullData[i][j] << "\t";
            }
            out << endl;
        }
        out << "\n=== НОВЫЕ ХАРАКТЕРИСТИКИ ===" << endl;
        out << "Максимум: " << getMax() << endl;
        out << "Минимум: " << getMin() << endl;
        out << "Диапазон: " << getRangeInShaded() << endl;
        out.close();
    }
};


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int n;
    cout << "Введите размер матрицы (нечетное число >= 11): ";
    cin >> n;

    DynamicSquareMatrix m(n);
    m.printToConsole();
    m.saveOriginalToFile("matrix.txt");
    m.deleteRowWithMaxInShadedArea();
    m.saveResultToFile("matrix_result.txt");
    return 0;
}

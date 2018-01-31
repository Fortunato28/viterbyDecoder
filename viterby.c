#include <stdio.h>
#include <viterby.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief количество существующих состояний
 * количество возможных состояний S рассчитывается как 2^m, где
 * m - размер регистра кодирования. В соответствии с ITU-T m = K - 1,
 * т.е. m = 6. Таким образом, количество состояний S = 2^6 = 64.
 * Каждому состоянию S соответствует своя выходная последовательность
 * кодовых символов.
 */
#define S 64

/**
 * @brief массив переходов конечного автомата
 *        Количество возможных состояний конечного автомата S.
 *        Каждое состояние автомата соответствует конкретному состоянию регистра
 *        кодера. Массив реализован в соответствии с логикой структуры данных
 *        "двоичное дерево", а значит относительно любого состояния можно найти
 *        по индексу (i) его левого потомка (2i) и правого потомка (2i+1).
 *        Также дерево закольцовано, что позволяет перемещаться по нему по модулю 32 (S/2).
 */
size_t jumpArray[S];

/**
 * @brief инициализаци¤ массива переходов
 */
void initJumpArray()
{
    size_t i;
    for(i = 0; i < S; ++i)
    {
       jumpArray[i] = i;
    }
}

/**
 * @brief функция запускает декодирование слова по алгоритму Витерби
 * @param
 *  codedStream - закодированная информация
 *  lenCoded - количество переданной информации (битах)
 *  decodedStream - декодированные данные
 *  lenDecodedStream - размер выходных декодированных данных
 */
void getDecode(void *codedStream, size_t lenCoded,
               void *decodedStream, size_t lenDecoded)
{
    initJumpArray();
    size_t timeStep = lenCoded / 2; 	// Высота дерева-пути, а также количество временных шагов
    uint32_t treeHeight = 0;			// Текущая высота построенного дерева
    sPath path[S];  					// Массив всех возможных путей
    uint32_t pathSize = 0;		 		// Размер массива path

    uint32_t depth; 					// Глубина поиска
    bool wasVisited; 					// Флаг, отмечающий, был ли узел дерева путей уже посещён
    bool isFirst; 						// Флаг, отмечающий, что дерево путей создано впервые

    uint32_t mostProbably[timeStep]; 	// Узлы наиболее вероятного пути
    uint32_t index = 0; 				// Текущий индекс массива, содержащего наиболее вероятный путь

    while(treeHeight < timeStep)
    {
        clearPath(path, pathSize++);	// Очищение массива путей

        treeHeight++; 					// Remove!
    }


    //printf("Consistently: %d, %d, %d, %d\n", *(uint32_t *)codedStream, timeStep,  *(uint32_t *)decoded, lenDecoded);
}

/**
 * @brief функция запускает декодирование слова по алгоритму Витерби
 * @param
 *  coded - закодированная информация
 *  lenCoded - количество переданной информации (битах)
 *  decoded - декодированные данные
 *  lenDecoded - размер выходных декодированных данных
 */

void clearPath(sPath path[], size_t pathSize)
{
    size_t i; // Итератор по массиву вероятных путей
    for(i = 0; i < pathSize; ++i)
    {
        path[i].metric = 0;
        size_t j; // Итератор по массиву пройденных узлoв
        for(j = 0; j < path[i].nodeSize; ++j)
        {
            path[i].currentNode[j] = 0;
        }
        path[i].nodeSize = 0;
    }
}





































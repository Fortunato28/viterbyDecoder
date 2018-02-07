#include <stdio.h>
#include <viterby.h>
#include <stdint.h>
#include <stdbool.h>


//******************************Удалить*******************************************
bool codeTable[2*S][N] =
{   0,	0,
    1,	1,
    1,	0,
    0,	1,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    1,	0,
    0,	1,
    0,	0,
    1,	1,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    0,	1,
    1,	0,
    1,	1,
    0,	0,
    1,	0,
    0,	1,
    0,	0,
    1,	1
};
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
    size_t treeHeight = 0;			// Текущая высота построенного дерева
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
        pathSize = 0;					// Сброс размера массива путей
        depth = 0;						// Сброс глубины поиска
        viterby(pathTree, &treeHeight, &depth, codedStream, lenCoded);		// Вызов функции, реализующей алгоритм Витерби

        treeHeight++; // Remove!
    }


}

/**
 * @brief функция, очищающая массив вероятных путей
 * @param
 *  sPath path[S] - массив верояётных путей
 *  pathSize - размер массива вероятныз путей
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

/**
 * @brief функция получения вероятных путей по алгоритму Витерби
 * @param
 *  tree - указатель на текущий узел дерева возможных путей
 *  treeHight - текущая высота дерева (индекс массива последовательносте из N-символов)
 *  depth - глубина поиска
 *  codedStream - закодированная информация
 *  len - количество переданной информации (в битах)
 */
sTree *viterby(sTree *tree, size_t *treeHight, uint32_t *depth, void *codedStream, size_t len)
{
    size_t counter = 0;			// Счётчик количества несовпадений
    bool left = true;			// Флаг, определяющий, какой потомок добавится: левый или правый
    uint32_t state;				// Текущее состояние кодера

    if(tree)					// Дерево путей не пустое
    {
        state = tree->data;		// Установка начального состояния в значение последнего узла дерева
    }
    else
    {
        state = 0;				// Начинаем с нулевого состояния
    }
    if(*depth < DEPTH)
    {
        size_t i;
        size_t j;
        size_t index = jumpArray[2*state];  				// Запись возможного перехода в левого потомка

        selectDibits(dibit, treeHight, codedStream, len);	// Выделение битов для сравнения

        if(!memcmp(dibit, codeTable[index], N))				// При совпадении текущих двух битов с битами, получаемыми при возможном переходе
        {
            counter = 0;
            tree = addNode(index, treeHight, tree, left);	// Добавляем узел
            ++*depth;
            ++*treeHight;									// Дерево стало выше
            return tree;
        }
        else
        {
            counter++;
            left = false; 			// Инвертирование флага, определяющего потомка



            /*********************Delete********************/
            counter = 2;
            index 	= 0;
            /*********************Delete********************/



            if(counter == N)
            {
                counter = 0;			// Сброс счётчика несовпадений
                tree = addNode(index, treeHight, tree, false); 		// Добавляем правое поддерево
                ++*depth;
                hammingCounter(tree, dibit, codeTable[index]);
            }
        }
    }
}

/**
 * @brief функция выделения двух текущих битов закодированной последовательности,
 *  	  необходимых для сравнения со значениями переходов при построении кодовой решетки
 * @param
 *	dibit - массив, в который будут помещены бита
 *  treeHight - текущая высота дерева (текущая двойка битов)
 *  codedStream - закодированная информация
 *  len - количество переданной информации (в битах)
 */
void selectDibits(bool dibit[N], uint32_t *treeHight, void *codedStream, size_t len)
{
    size_t i;
    size_t offset = len - (N+2*(*treeHight)); 	// Смещение двух битов в младшие разряды

    for(i = 0; i < N; ++i)
    {
        dibit[i] = (*(uint32_t *)codedStream >> (offset + !i)) & LOW_BIT;
    }
}

/**
 * @brief функция добавляет в дерево вероятных путей новый узел
 * @param
 *  state - значение нового узла дерева
 *  treeHight - индекс массива последовательностей, при котором произошло ветвление
 *  node - указатель на новый узел дерева
 *  left -  флаг, указывающий, в какое поддерево будет добавлен нвый узел.
 *          При left = true узел будет добавлен в левое поддерево, иначе - в правое.
 */
sTree *addNode(size_t state, size_t treeHight, sTree *node, bool left)
{

   if(!node) 			// если узел не существует
   {
       node = (sTree *)malloc(sizeof(sTree));
       node->data = state;
       node->metric = 0;
       node->codeIndex = treeHight;	// Запись индекса ветвления узла
       node->left = NULL;			// Инициализация указателя на левого потомка
       node->right = NULL;
       node->parent = NULL;
   }
   else
   {
        if(left)   // Создание левого или правого поддерева-потомка
        {
            node->left = createBranch(state, treeHight, node->left, node, left);
        }
        else
        {
            node->right = createBranch(state, treeHight, node->right, node, left);
        }
   }
   return node;
}

/**
 * @brief функция добавляет в поддерево вероятных путей новый узел
 * @param
 *  state - значение нового узла дерева
 *  treeHight - индекс массива последовательностей, при котором произошло ветвление
 *  node - указатель на новый узел дерева
 *  left -  флаг, указывающий, в какое поддерево будет добавлен новый узел.
 *  parent - указатель на родительский узел
 */
sTree *createBranch(size_t state, size_t treeHight,
                     sTree *node, sTree *parent, bool left)
{
     node = addNode(state, treeHight, node, left); 		// Добавление нового узна в поддерево
     node->parent = parent;
     return node;
}

/**
 * @brief фкнуция подсчета метрики Хэмминга.
 *        Метрика Хэмминга рассчитывается как количество
 *        различающихся символов в двух массивах.
 * @param
 *  tree 	- указатель на текущий узел дерева путей
 *  array_1 - указатель на первый массив
 *  array_2 - указатель на второй массив
 */
void hammingCounter(sTree *tree, bool *array_1, bool *array_2)
{
    size_t i;
    size_t counter = 0;
    for(i = 0; i < N; ++i)
    {
        if(array_1[i] != array_2[i])
        {
            ++counter;
        }
    }
        tree->metric = counter;
}































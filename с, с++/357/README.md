### **Задача:**
Построить последовательность до n-ного элемента, чтобы каждый из них делился на 3,5,7.  
Необходимо разработать программу с оптимальным хранением в памяти с использованием линейного списка. Программа выводит n-ый элемент (n введено с клавиатуры) из заданной выше последовательности.
### Основная часть

Алгоритм строится на 3 очередях и функциях к ним. Все элементы в одной очереди делятся на 3,5 или 7 в зависимости от очереди.
Функция minna() снимает нужный элемент и добавляет в концы очередей производные от него. Она состоит из 7 условий, чтобы просчитать все случаи минимального элемента в начале очереди и проверки, отсекающей повторные числа.
### Анализ работы
В ходе работы были закреплены навыки работы с основными структурами данных в языке С. Можно сделать вывод о том, что конечный вариант программы является неоптимизированным, так как программа работает со сложностью (O(n)).
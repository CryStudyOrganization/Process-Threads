## Лабораторна робота №1: "Процеси та потоки"
# [ЗВІТ ТУТ](https://github.com/CryStudyOrganization/Process-Threads)
### Тема:
"Процеси та потоки. Взаємодія між процесами. Розподіл даних між процесами. Робота з файлами, що відображуються в пам'ять."

### Завдання 1:
1. Написати дві програми, які будуть мати спільні дані та одночасно до них звертатися.
2. Використати проектування файлу в пам'ять для спільного доступу до даних.
3. Одна програма сортує дані у файлі, а інша відображає вміст цього файлу. Робота обох програм відбувається одночасно.
4. Створити файл `data.dat` із випадковими числами (20-30 штук, діапазон від 10 до 100).

### Програма №1: "Сортування даних" (консольна)
Примітка: програмаа може бути як віконною так і консольною, та може включати в себе генерацію чисел напряму. Якщо це так, то буде дві програми, в іншому випадку за генерацію данних буде відповідати третя програма.
- Програма сортує дані у файлі `data.dat`, використовуючи будь-який метод сортування та додає затримку 1 секунду на кожній ітерації сортування.
- Використовує проектування файлу в пам'ять для доступу до даних.
- Після завершення сортування виводить повідомлення "Робота завершена".

### Програма №2: "Виведення файлу даних у вікно" (віконна)

- Програма відображає дані з файлу `data.dat` у вікні.
- Використовує проектування файлу в пам'ять для доступу до даних.
- Використовує таймер з інтервалом 0.5 секунди для відображення даних.
- У вікно виводяться рядки одного символу, які відповідають числам з файлу.

### Завдання 2: Синхронізація процесів

- Додати синхронізацію потоків, які звертаються до спільних даних.
- Використовувати об'єкти синхронізації, такі як mutex або semaphor, для коректної взаємодії програм.
- Обробити виняткові ситуації та визначити критичні секції коду для усіх програм.

### Додаткове завдання: Програма №3 (консольна)

- Написати третю програму, яка одночасно запускається та намагається відсортувати той самий масив у протилежному напрямку, використовуючи інший метод сортування.

### Завдання підвищеної складності: Робота по мережі

- Організувати роботу всіх чотирьох програм на різних робочих станціях локальної мережі, з відображенням одного того самого файлу даних.
- Вивчити, як зробити видимим об'єкт "відображений файл" у локальній мережі.

## Структура папок

- **DataManager**: Містить програму для сортування та доступу до спільних даних.
- **DataTimSort**: Можлива програма для додаткового завдання, сортування в іншому напрямку.
- **DataView**: Містить програму для відображення даних у вікно.
- **datafiles**: Тут міститься файл `data.dat` із випадковими числами.

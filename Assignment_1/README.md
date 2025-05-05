
# Author: Kotegov Nikita | B81 | st128906@student.spbu.ru

* Поддерживается потоковая работа через `STDIN`/`STDOUT`
* При ошибке формата декодер завершается с ненулевым кодом  
  (проверяется в CI на GitHub Actions)
---
## Сборка

cd Assignment_1
g++ -std=c++17 -O2 -Wall -Wextra ascii85.cpp ascii85_cli.cpp -o ascii85


# Ручной ввод (Ctrl+D — конец ввода)
printf "Hello" | ./ascii85
printf "<~87cURD]j7BEbo80~>" | ./ascii85 -d


# ДЗ 2. Паралельне обчислення числа pi методом Монте-Карло

## Що реалізовано

Програма обчислює pi методом Монте-Карло для різних значень `N` і кількості потоків `M`.

Повний режим використовує:

```text
N = 1_000_000, 10_000_000, 100_000_000, 1_000_000_000, 10_000_000_000, 100_000_000_000
M = 1, 2, 4, 8, 16, 32, 64, 128
```

Кількість точок передається потокам рівними батчами. Якщо `N` не ділиться на `M` рівно, залишок розподіляється по перших потоках.

Результат записується у файл:

```text
results/pi_monte_carlo_parallel_results.csv
```

У CSV значення `N` розміщені по вертикалі, а `M` по горизонталі. На перетині записаний час виконання у секундах.

## Структура

```text
.
├── Makefile
├── README.md
├── src
│   └── main.cpp
└── results
    └── pi_monte_carlo_parallel_results.csv
```

## Швидка перевірка

```bash
make
./build/hw2_parallel_pi --quick
```

або:

```bash
make run
```

## Повний запуск для здачі

```bash
make
./build/hw2_parallel_pi --full
```

Після повного запуску потрібно додати оновлений CSV у коміт:

```bash
git add results/pi_monte_carlo_parallel_results.csv
git commit -m "Add full parallel pi results"
git push
```

## Завантаження на GitHub

```bash
git init
git branch -M main
git add .
git commit -m "Add homework 2 parallel Monte Carlo pi"
git remote add origin https://github.com/YOUR_LOGIN/hw2-parallel-monte-carlo-pi.git
git push -u origin main
```

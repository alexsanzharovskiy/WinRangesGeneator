# WinRangesGenerator
Генератор таблиц win ranges (C++/PHP). Используется для MathEngine/Simulator.

## Параметры

| Позиция | Значение | Описание |
|---|---|---|
| 1 | `maxWin` | Максимальный множитель |
| 2 | `minStep` | Минимальный шаг выплат (стартовая граница) |
| 3 | `maxMultiplier` | Порог, до которого диапазоны точные (start:end) |
| 4 | `ranges` | Правила роста шага: `{from}:{inc};...` (пример: `10:5;2000:10;1000:20`) |
| 5 | `output_file` | (опционально) имя файла, по умолчанию `win_ranges_base.json` |

## C++ usage

Сборка:
```bash
g++ -std=c++11 -o WinRangesGenerator WinRangesGenerator.cpp
```

Запуск:
```bash
./WinRangesGenerator 5000 0.25 10 '10:5;2000:10;1000:20' win_ranges_base.json
```

## PHP usage
```php
$service = new WinRangesGenerator($maxWin, $minStep, $maxMultiplier, $ranges);
$service->generateJsonFile();
```

## Формат результата
JSON-массив вида:
```json
{
  "payout": 12.5,
  "ranges": [10, 15]
}
```
Первый диапазон начинается строго с `minStep`, далее шаги увеличиваются по правилам `ranges`.

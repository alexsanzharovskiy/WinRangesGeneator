#Win ranges generator (PHP/C++) versions.
Can be used for RtpCalculator

|  Value |  Description | Variable name |
|---|---|---|
|  5000 |  Max winning multiplier| $maxWin |
|  0.25 |  Minimum winning (minimal step of increasing ranges)| $minStep |
|  10 | Maximum multiplier for exact start:end in ranges  | $maxMultiplier|
|  10:5;2000:10;1000:20 | Next step is to provider multiplier ranges for generator. In format: {fromMultiplier1}:{increase1};{fromMultiplier2}:{increase2}..., Example:10:5;200:10;1000:20  | $ranges | 

--------
## C++ Usage 

1) How to compile file:
   1) ``g++ -std=c++11 -o WinRangesGenerator WinRangesGenerator.cpp ``


2) How to run Command:
``./WinRangesGenerator 5000 0.25 10 '10:5;2000:10;1000:20'``

------------

##PHP Usage
```php
<?php
 $service = new WinRangesGenerator(
    $maxWin,
    $minStep,
    $maxMultiplier,
    $ranges
);

$service->generateJsonFile();

```
-------------

As result of command execution

```json
[
    {"payout":0,"ranges":[0,0]},
    {"payout":0.25,"ranges":[0.25,0.25]},
    {"payout":0.50,"ranges":[0.50,0.50],
    ...
    {"payout":10,"ranges":[10,10]},
    {"payout":12.50,"ranges":[10,15]},
    {"payout":17.50,"ranges":[15,20]},
    ...
    {"payout":4997.50,"ranges":[4995,5000]},
    {"payout":5000,"ranges":[5000,5000]}
]


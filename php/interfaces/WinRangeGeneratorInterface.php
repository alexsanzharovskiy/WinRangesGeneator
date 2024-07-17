<?php

interface WinRangeGeneratorInterface
{
    public function __construct($maxWin, $minStep, $exactStepLimitValue, array $rangeSteps);

    public function generateJsonFile(string $savePath): bool;
}

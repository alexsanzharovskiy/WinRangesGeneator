<?php

use interfaces\WinRangeGeneratorInterface;

class WinRangesGenerator implements WinRangeGeneratorInterface
{
    public const PAYOUT = 'payout';
    public const RANGES = 'ranges';

    public const JSON_FILE_NAME = 'win_ranges.json';

    private float $start = 0.00;
    private float $end = 0.00;
    private array $rangeSteps;
    private int $exactStepLimitValue;
    private float $minStep;
    private int $maxWin;

    private string $savePath = '';

    private array $generatedRanges = [];

    /**
     * @param int $maxWin
     * @param float $minStep
     * @param int $exactStepLimitValue
     * @param array $rangeSteps
     */
    public function __construct( $maxWin,  $minStep,  $exactStepLimitValue, array $rangeSteps)
    {
        $this->maxWin = $maxWin;
        $this->minStep = $minStep;
        $this->exactStepLimitValue = $exactStepLimitValue;
        $this->rangeSteps = $rangeSteps;
    }

    /**
     * @param string $savePath
     * @return bool
     */
    public function generateJsonFile(string $savePath = ''): bool
    {
        if (empty($this->generatedRanges)) {
            $this->generate();
        }

        $savePath = $savePath ?: dirname(__FILE__) . '/storage/' . self::JSON_FILE_NAME;
        $this->savePath = $savePath;

        return (bool) file_put_contents($savePath, json_encode($this->generatedRanges));
    }

    /**
     * @return string
     */
    public function getSavePath(): string
    {
        return $this->savePath;
    }

    private function generate(): void
    {
        $this->generatedRanges[] = [
            self::PAYOUT => 0,
            self::RANGES => [0, 0]
        ];

        $currentStepIndex = 0;
        $keys = array_keys($this->rangeSteps);
        while ($this->end < $this->maxWin) {
            if ($this->end < $this->exactStepLimitValue) {
                $this->start += $this->minStep;
                $this->end += $this->minStep;
            } else {
                if ($currentStepIndex < count($this->rangeSteps) - 1 && $this->end >= $this->rangeSteps[$keys[$currentStepIndex+1]]) {
                    $currentStepIndex++;
                }
                $this->start = $this->end;
                $this->end += $this->rangeSteps[$keys[$currentStepIndex]];
            }

            $this->generatedRanges[] = [
                self::PAYOUT => $this->start < $this->exactStepLimitValue ? $this->start : $this->start + ($this->end - $this->start) / 2,
                self::RANGES => [$this->start, $this->end]
            ];
        }

        $this->generatedRanges[] = [
            self::PAYOUT => $this->maxWin,
            self::RANGES => [$this->maxWin, $this->maxWin]
        ];
    }
}

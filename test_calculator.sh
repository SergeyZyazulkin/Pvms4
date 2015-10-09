sudo sh load_calculator.sh

echo "1" > /dev/calculator_first
echo "2" > /dev/calculator_second
echo "+" > /dev/calculator_operation
result=$(cat /dev/calculator_result)
echo "1 + 2 = $result"

echo "5" > /dev/calculator_first
echo "10" > /dev/calculator_second
echo "-" > /dev/calculator_operation
result=$(cat /dev/calculator_result)
echo "5 - 10 = $result"

echo "3" > /dev/calculator_first
echo "6" > /dev/calculator_second
echo "*" > /dev/calculator_operation
result=$(cat /dev/calculator_result)
echo "3 * 6 = $result"

echo "1" > /dev/calculator_first
echo "0" > /dev/calculator_second
echo "/" > /dev/calculator_operation
result=$(cat /dev/calculator_result)
echo "1 / 0 = $result"

echo "-12" > /dev/calculator_first
echo "2" > /dev/calculator_second
echo "/" > /dev/calculator_operation
result=$(cat /dev/calculator_result)
echo "-12 / 2 = $result"

echo "a" > /dev/calculator_first
echo "2" > /dev/calculator_second
echo "*" > /dev/calculator_operation
result=$(cat /dev/calculator_result)
echo "a * 2 = $result"

echo "3" > /dev/calculator_first
echo "a" > /dev/calculator_second
echo "/" > /dev/calculator_operation
result=$(cat /dev/calculator_result)
echo "3 / a = $result"

sudo sh unload_calculator.sh

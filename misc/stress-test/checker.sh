set -e
g++ -std=c++20 TestGenerator.cpp -o TestGenerator
g++ -std=c++20 SolnCorrect.cpp -o SolnCorrect
g++ -std=c++20 SolnMy.cpp -o SolnMy
for((i = 1;i<=1000000 ; ++i)); do
    ./TestGenerator $i > input_file
    ./SolnMy < input_file > AnswerMy
    ./SolnCorrect < input_file > AnswerCorrect
    diff -Z AnswerMy AnswerCorrect > /dev/null || exit
    echo "Passed test: "  $i
done
echo "WA on the following test:"
cat input_file
echo "Your answer is:"
cat AnswerMy
echo "Correct answer is:"
cat AnswerCorrect
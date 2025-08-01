int main()
{
    int numbers[10], matrix[20];
    float values[5];
    bool flags[3];
    int i, total, temp;
    
    i = 0;
    numbers[0] = 42;
    numbers[1] = numbers[0];
    numbers[i] = 100;
    
    values[0] = 3.14;
    values[1] = 2.71;
    
    flags[0] = true;
    flags[1] = false;
    flags[2] = flags[0];
    
    temp = numbers[0];
    
    return 0;
}

string substring ( string * str, int beginIndex, int endIndex)
{
    // preconditions
    assert(str != NULL);
    assert(str->content != NULL);
    assert(beginIndex >= 0 && beginIndex <= endIndex);
    assert(endIndex < str->length);

    int index = 0; // index for traverse the result array
    string *result = malloc(sizeof(string));
    result->length = endIndex - beginIndex; // length of the substring
    result->content = malloc( result->length+1 * sizeof(char) ); // add 1 for the string terminator

    for(int i=beginIndex; i <= endIndex; i++){
        result->content[index++] = str->content[i];
    }

    result->content[index] = '\0'; // string termintor

    // postconditions
    assert(result->content != NULL);
    assert(strlen(result->content) == result->length);

    return result;
}

int removeFirstItem()
{
    // preconditions
    validate_list(top); 
    assert( top != NULL && numNodes > 0 );

    int result = 0; // default false

    if(top != NULL && numNodes > 0){
        node *prevTop = top; // a pointer keep track of the soon to be previous top node
        top = top->next; // the next node is now the new top node
        
        // free the previous top node
        prevTop->next = NULL;
        free(prevTop); 
        
        int numNodes--; // decrease the number of nodes by 1
        validate_list(top); /// postcondition

        result = 1; // removed the first node successfully
    }

    return result;
}
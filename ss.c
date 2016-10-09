/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: ?
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
SymbolTable *stable_create ()
{
	SymbolTable *st;
	st = emalloc(sizeof(SymbolTable));
	return st;
};

SymbolTable *stable_find (SymbolTable *st, const char *key)
{
	if (st == NULL)
		return NULL;

	if (strcmp(key, st->data.str) == 1)
		return stable_find(st->right, key);

	else if (strcmp(key, st->data.str) == -1)
		return stable_find(st->left, key);

	else
		return st;
}

SymbolTable *stable_insert (SymbolTable *st, const char *key)
{
	if (st == NULL){
		SymbolTable *temp;
		temp = stable_create();
		temp->left = temp->right = NULL;
		temp->data.str = strdup(key);
		return temp;
	}
	
	if (strcmp(key, st->data.str) > 0)
		st->right = stable_insert(st->right, key);

	else if (strcmp(key, st->data.str) < 0)
		st->left = stable_insert(st->left, key);

	return st;
}

void printInOrder (SymbolTable *st)
{
	if (st == NULL)
		printf("");
	else {
		printInOrder(st->left);
		printf("%s ", (st->data).str);
		printInOrder(st->right);
	}
}

// Example test file for use in computing equations for "Count-controlled loops"
// Note that many "Condition-controled loops" will be more complex cases to 
// generate equations (where possible).  High level abstractions will, in many
// cases, provide the semantics to define equations for such 
// "Condition-controled loops".

// Example equations generated by Maple:
//    Text version:
//       19+13*loop_expression0x8217f78+4*loop_expression0x8217f78^2+19*loop_expression0x8217fa0+10*loop_expression0x8217fa0^2
//    Latex version:
//       19+13\,{\it loop\_expression0x8217f78}+4\,{{\it loop\_expression0x8217f78}}^{2}+19\,{\it loop\_expression0x8217fa0}+10\,{{\it loop\_expression0x8217fa0}}^{2}

int base  = 0;
int bound;
int x;
int array[100];

#if 1
int foobar(int bound1, int bound2)
   {
  // int x = 0;
  // x = x + x + x;

#if 1
     for (int i = 0; i < bound1; i++)
        {
          array[i] = 0;
#if 1
          for (int j = 0; j < i; j++)
             {
               x = 0;
             }
#endif
        }
#endif

#if 1
     for (int i = 0; i < bound2; i++)
        {
          array[i] = 0;
#if 1
          for (int j = 0; j < bound2; j++)
             {
               x = 0;
             }
#endif
        }
#endif

     return x;
   }
#endif

int main()
   {

#if 1

#if 1
  // x = 4;
     x = foobar(bound,bound);
#endif

     array[0] = 0;

#if 0
  // int n;
     for (int i = 0; i < bound; i++)
        {
           array[i] = 0;
#if 0
          for (int j = 0; j < bound; j++)
             {
               x = foobar(bound,bound);
             }
#endif
        }
#endif

#if 0
     for (int i = 7; i < 42; i++)
        {
           x = i;
          for (int j = 2; j < 42; j++)
             {
               x = j;
             }
        }

     for (int i = 7; i < n; i++)
        {
           x = i;
          for (int j = 2; j < n; j++)
             {
               x = j;
             }
        }

     for (int i = 7; i < n; i++)
        {
           x = i;
          for (int j = 2; j < i; j++)
             {
               x = j;
             }
        }

     for (int i = 7; i < n; i++)
        {
           x = i;
          for (int j = 2; j < i; j++)
             {
               x = j;
             }
        }
#endif

#endif

     return 0;
   }



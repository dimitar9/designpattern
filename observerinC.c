#include <stdio.h>
#include <stdlib.h>

/* observed component */
typedef struct
{
  int listener_count;
  /* array of function pointers */
  void (*listener_list[10])(unsigned,unsigned);
} Button;

void button_Init( Button *pButton )
{
  pButton->listener_count = 0;
}

void button_RemoveClickListener( Button *pButton,
                                 void (*fp)(unsigned,unsigned) )
{
  unsigned i;
  void (*temp)(unsigned,unsigned);

  for( i = 0; i < pButton->listener_count; ++i )
  {
    if( pButton->listener_list[i] == fp )
    {
      pButton->listener_count--;
      pButton->listener_list[i] =
        pButton->listener_list[pButton->listener_count];
    }
  }
}

void button_AddClickListener( Button *pButton,
                              void (*fp)(unsigned,unsigned) )
{
  button_RemoveClickListener( pButton, fp );
  pButton->listener_list[ pButton->listener_count++ ] = fp;
}

void button_Click( Button *pButton, unsigned x, unsigned y )
{
  unsigned i;
  for( i = 0; i < pButton->listener_count; ++i )
  {
    pButton->listener_list[i](x,y);
  }
}

/* observer component 1 */
void observer_OnClick( unsigned x, unsigned y )
{
  printf( "observer_OnClick( 0x%X, 0x%X )\r\n", x, y );
}

/* observer component 2 */
bool observer2_isClickEventHandled = true;

void observer2_OnClick( unsigned x, unsigned y )
{
  observer2_isClickEventHandled = false;
}

void observer2_Action()
{
  if( observer2_isClickEventHandled )
  {
    printf( "observer2_Action(): no click event to handle\r\n" );
  }
  else
  {
    printf( "observer2_Action(): handling click event\r\n" );
  }
  observer2_isClickEventHandled = true;
}

/* system */
int main()
{
  Button button;

  /* system initialization */
  button_Init( &button );
  button_AddClickListener( &button, observer_OnClick );
  button_AddClickListener( &button, observer2_OnClick );

  /* system operation */
  observer2_Action();

  /* more system operation */
  button_Click( &button, 1,3);

  /* and more system operation */
  observer2_Action();
  observer2_Action();
}

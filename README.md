# 42ft_select
42 school's ft_select project

This project is a good introduction to termcaps.

The program takes a list of choices as arguments and present them to you.

You can:

- Move around using the cursor keys
- Delete choices using delete or backspace
- Select choices using the spacebar

Use of Ctrl-D or Enter to validate the choices and return to the shell,
or Esc to cancel.

If enough vertical space is available, the choices are displayed in a vertical list.
Otherwise, several columns are used, if possible.

If a choice doesn't fit in the horizontal space, it scrolls when the cursor is on it.

Example usage:

```
ls `./ft_select ~/*`
```
allows you brig a list of folders/files in your home, select the one you want, and return the results to ls, which will print the selected choice in ft_select.

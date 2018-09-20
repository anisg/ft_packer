SECTIONS
{
    .note : {
        *(.note.my-version)
    } :note
    .text : {
        *(.data)
    } :load
    _end = .;
}
PHDRS
{
  note PT_NOTE;
  load PT_LOAD;
}

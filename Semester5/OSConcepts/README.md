Demand Paging

Processor holds a logical view. You only bring in a page when you need it
    only when the page is actually needed do you bring it in

    Lazy Swapper- Don't do it until its absolutely critical
                Unless a page is needed in ram don't use it

    Something has to kick start the swapping process. 

    Almost everything is driven by interrupts.

*Page Fault*

    Instruction that requires access to a word in memory someone had to make a 
    reference to ao memory word. This is what starts of the process

    When you say memory ccess in the paging system what comes out?
        Ans: A page and an offeset.
        
    The page table converts a page number to a frame number. Uses the P to offeset N
        *Page Table* - That is i, or invalid. The frame doesnt exist
        
        **THis is what starts a page fault**

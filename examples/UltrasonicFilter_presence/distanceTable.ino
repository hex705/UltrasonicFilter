/* cool table for filtering 

-- with the Ultrasonic library / sensor combination we can set a timeout -- which is needed for filtering.
-- it needs t obe confirmed for this filter to work well
-- values below are tested

these values are determined empirically (by testing, not calculation) 

look at distances on left below use one in following line in setup

    usFilter.begin(maxDistance); 


maxDistance (cm)               corresponding timout setting 
you want to measure           (for reference only - do not set)

178                                 10000
196                                 11000
214                                 12000
232                                 13000
250                                 14000
268                                 15000
285                                 16000
303                                 17000

*/
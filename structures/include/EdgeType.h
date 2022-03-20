#ifndef EDGETYPE_H_INCLUDED
#define EDGETYPE_H_INCLUDED

/**
 * @brief Edge types
 * 
 */
enum edge_type_t : char{
    NO              = 'n',
    MOTORWAY        = 'M',
    MOTORWAY_LINK   = 'm',
    TRUNK           = 'T',
    TRUNK_LINK      = 't',
    PRIMARY         = 'P',
    PRIMARY_LINK    = 'p',
    SECONDARY       = 'S',
    SECONDARY_LINK  = 's',
    TERTIARY        = 'E',
    TERTIARY_LINK   = 'e',
    UNCLASSIFIED    = 'U',
    RESIDENTIAL     = 'R',
    LIVING_STREET   = 'L',
    SERVICE         = 'V'
};

#endif //EDGETYPE_H_INCLUDED

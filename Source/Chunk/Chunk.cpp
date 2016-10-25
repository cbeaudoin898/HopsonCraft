#include "Chunk.h"

#include <iostream>

#include "Block/Block.h"

#include "Loader.h"
#include "Height_Generator.h"

#include "Random.h"
#include "Block/D_Blocks.h"
#include "Chunk_Map.h"

Chunk::Chunk(const Chunk_Location& position, Chunk_Map& chunkMap, const Texture_Atlas& blockAtlas)
:   m_blocks        ( SIZE * SIZE * HEIGHT )
,   m_location      (position)
,   m_position      (position.x * SIZE, position.z * SIZE)
,   m_p_chunkMap    (&chunkMap)
,   m_p_atlas       (&blockAtlas)
{
    generateBlockData       ();
    generateStructureData   ();
}

void Chunk :: setBlock (const Vector3& position, Block::Block_Base& block, bool overrideBlocks)
{
}

void Chunk :: qSetBlock   (   GLuint x, GLuint y, GLuint z, Block_t& block, bool overrideBlocks )
{
    if ( !m_blocks.at( SIZE * SIZE * y + SIZE * x + z ) )
    {
        m_blocks.at( SIZE * SIZE * y + SIZE * x + z ) = &block;
    }
    else if ( getBlock( x, y, z).getID() == Block::ID::Air )
    {
        m_blocks.at( SIZE * SIZE * y + SIZE * x + z ) = &block;
    }
    else if ( overrideBlocks )
    {
        m_blocks.at( SIZE * SIZE * y + SIZE * x + z ) = &block;
    }
}



const Block_t& Chunk :: getBlock ( int x, int y, int z ) const
{
    if ( x == -1 )
    {
        return getAdjChunkBlock( -1, 0, SIZE - 1, y, z );
    }
    else if ( z == -1 )
    {
        return getAdjChunkBlock( 0, -1, x, y, SIZE - 1 );
    }
    else if ( x == SIZE )
    {
        return getAdjChunkBlock( 1, 0, 0, y, z );
    }
    else if ( z == SIZE )
    {
        return getAdjChunkBlock( 0, 1, z, y, 0 );
    }
    else if ( y == -1 || y > HEIGHT - 1 )
    {
        return Block::dirt;
    }
    else
    {
        return *m_blocks.at( SIZE * SIZE * y + SIZE * x + z );
    }
    return Block::dirt;    //This is for world edges.
}



const Block_t& Chunk :: getAdjChunkBlock ( int xChange, int zChange, int blockX, int blockY, int blockZ ) const
{
    Chunk_Location location ( m_location.x + xChange, m_location.z + zChange);

    if (m_p_chunkMap->getChunkAt(location))
    {
        return m_p_chunkMap->getChunkAt(location)->getBlock({blockX, blockY, blockZ});
    }
    else return Block::dirt;
}



const Block_t& Chunk :: getBlock ( const Vector3& location ) const
{
    return getBlock( location.x, location.y, location.z );
}

bool Chunk :: hasVertexData () const
{
    return m_hasVertexData;
}


bool Chunk :: hasBlockData   () const
{
    return m_hasBlockData;
}


bool Chunk :: hasBuffered () const
{
    return m_hasBufferedData;
}


const Chunk_Location& Chunk :: getLocation () const
{
    return m_location;
}


const Vector2& Chunk :: getPosition () const
{
    return m_position;
}

const Model& Chunk :: getChunkModel  () const
{
    return m_solidPart.model;
}


const Model& Chunk :: getWaterModel  () const
{
    return m_waterPart.model;
}


const Model& Chunk :: getFloraModel  () const
{
    return m_solidPart.model;
}


void Chunk :: setToDelete ()
{
    m_shouldBeDeleted = true;
}


bool Chunk :: hasDeleteFlag ()
{
    return m_shouldBeDeleted;
}



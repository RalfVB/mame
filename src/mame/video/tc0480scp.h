// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria
#ifndef MAME_VIDEO_TC0480SCP_H
#define MAME_VIDEO_TC0480SCP_H

#pragma once

class tc0480scp_device : public device_t
{
public:
	tc0480scp_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// configuration
	template <typename T> void set_gfxdecode_tag(T &&tag) { m_gfxdecode.set_tag(std::forward<T>(tag)); }
	void set_gfx_region(int gfxregion) { m_gfxnum = gfxregion; }
	void set_tx_region(int txregion) { m_txnum = txregion; }
	void set_col_base(int col) { m_col_base = col; }
	void set_offsets(int x_offset, int y_offset)
	{
		m_x_offset = x_offset;
		m_y_offset = y_offset;
	}
	void set_offsets_tx(int x_offset, int y_offset)
	{
		m_text_xoffs = x_offset;
		m_text_yoffs = y_offset;
	}
	void set_offsets_flip(int x_offset, int y_offset)
	{
		m_flip_xoffs = x_offset;
		m_flip_yoffs = y_offset;
	}

	/* When writing a driver, pass zero for the text and flip offsets initially:
	then tweak them once you have the 4 bg layer positions correct. Col_base
	may be needed when tilemaps use a palette area from sprites. */

	u16 ram_r(offs_t offset);
	void ram_w(offs_t offset, u16 data, u16 mem_mask);
	u16 ctrl_r(offs_t offset);
	void ctrl_w(offs_t offset, u16 data, u16 mem_mask);

	void tilemap_update();
	void tilemap_draw(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer, int flags, uint32_t priority);

	/* Returns the priority order of the bg tilemaps set in the internal
	register. The order in which the four layers should be drawn is
	returned in the lowest four nibbles  (msn = bottom layer; lsn = top) */
	int get_bg_priority();

	/* Undrfire needs to read this for a sprite/tile priority hack */
	u8 pri_reg_r();

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_post_load() override;

private:
	// internal state
	u16                m_ctrl[0x18];

	std::vector<u16>   m_ram;
	u16 *              m_bg_ram[4];
	u16 *              m_tx_ram;
	u16 *              m_char_ram;
	u16 *              m_bgscroll_ram[4];
	u16 *              m_rowzoom_ram[4];
	u16 *              m_bgcolumn_ram[4];
	int                m_bgscrollx[4];
	int                m_bgscrolly[4];
	int                m_pri_reg;

	/* We keep two tilemaps for each of the 5 actual tilemaps: one at standard width, one double */
	tilemap_t          *m_tilemap[5][2];
	int32_t            m_dblwidth;

	int                m_gfxnum;
	int                m_txnum;
	int                m_x_offset, m_y_offset;
	int                m_text_xoffs, m_text_yoffs;
	int                m_flip_xoffs, m_flip_yoffs;

	int                m_col_base;

	required_device<gfxdecode_device> m_gfxdecode;

	void common_get_tc0480bg_tile_info( tile_data &tileinfo, int tile_index, u16 *ram, int gfxnum );
	void common_get_tc0480tx_tile_info( tile_data &tileinfo, int tile_index, u16 *ram, int gfxnum );

	TILE_GET_INFO_MEMBER(get_bg0_tile_info);
	TILE_GET_INFO_MEMBER(get_bg1_tile_info);
	TILE_GET_INFO_MEMBER(get_bg2_tile_info);
	TILE_GET_INFO_MEMBER(get_bg3_tile_info);
	TILE_GET_INFO_MEMBER(get_tx_tile_info);

	void dirty_tilemaps();
	void set_layer_ptrs();
	void bg01_draw( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer, int flags, uint32_t priority );
	void bg23_draw( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer, int flags, uint32_t priority );
};

DECLARE_DEVICE_TYPE(TC0480SCP, tc0480scp_device)

#endif // MAME_VIDEO_TC0480SCP_H

// license:BSD-3-Clause
// copyright-holders:Bryan McPhail
/***************************************************************************

  Dec0 Video emulation - Bryan McPhail, mish@tendril.co.uk

*********************************************************************/

#include "emu.h"
#include "includes/dec0.h"


/******************************************************************************/

/******************************************************************************/


uint32_t dec0_state::screen_update_hbarrel(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
	m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, 0x08, 0x0f);
	m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);

	/* HB always keeps pf2 on top of pf3, no need explicitly support priority register */

	m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, 0x00, 0x0f);
	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}

/******************************************************************************/

uint32_t dec0_state::screen_update_bandit(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	if (m_pri==0)
	{
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
		m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);
		m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}
	else
	{
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
		m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
		m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);
	}
	return 0;
}

/******************************************************************************/

uint32_t dec0_state::screen_update_baddudes(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	/* WARNING: inverted wrt Midnight Resistance */
	if ((m_pri & 0x01) == 0)
	{
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);

		if (m_pri & 2)
			m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0,0x08,0x08,0x08,0x08); // upper 8 pens of upper 8 priority marked tiles /* Foreground pens only */

		m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);

		if (m_pri & 4)
			m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,0,0x08,0x08,0x08,0x08); // upper 8 pens of upper 8 priority marked tiles /* Foreground pens only */
	}
	else
	{
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);

		if (m_pri & 2)
			m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,0,0x08,0x08,0x08,0x08); // upper 8 pens of upper 8 priority marked tiles /* Foreground pens only */

		m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);

		if (m_pri & 4)
			m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0,0x08,0x08,0x08,0x08); // upper 8 pens of upper 8 priority marked tiles /* Foreground pens only */
	}

	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}

/******************************************************************************/

uint32_t dec0_state::screen_update_robocop(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	int trans;

	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	if (m_pri & 0x04)
		trans = 0x08;
	else
		trans = 0x00;

	if (m_pri & 0x01)
	{
		/* WARNING: inverted wrt Midnight Resistance */
		/* Robocop uses it only for the title screen, so this might be just */
		/* completely wrong. The top 8 bits of the register might mean */
		/* something (they are 0x80 in midres, 0x00 here) */
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);

		if (m_pri & 0x02)
			m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, trans, 0x0f);

		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}
	else
	{
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);

		if (m_pri & 0x02)
			m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, trans, 0x0f);

		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}

	if (m_pri & 0x02)
		m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, trans^0x08, 0x0f);
	else
		m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);

	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}


uint32_t dec0_automat_state::screen_update_automat(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	int trans;

	// layer enables seem different... where are they?

	// the bootleg doesn't write these registers, I think they're hardcoded?, so fake them for compatibility with our implementation..
	m_tilegen[0]->pf_control_0_w(0,0x0003, 0x00ff); // 8x8
	m_tilegen[0]->pf_control_0_w(1,0x0003, 0x00ff);
	m_tilegen[0]->pf_control_0_w(2,0x0000, 0x00ff);
	m_tilegen[0]->pf_control_0_w(3,0x0001, 0x00ff); // dimensions

	m_tilegen[1]->pf_control_0_w(0,0x0082, 0x00ff); // 16x16
	m_tilegen[1]->pf_control_0_w(1,0x0000, 0x00ff);
	m_tilegen[1]->pf_control_0_w(2,0x0000, 0x00ff);
	m_tilegen[1]->pf_control_0_w(3,0x0001, 0x00ff); // dimensions

	m_tilegen[2]->pf_control_0_w(0,0x0082, 0x00ff); // 16x16
	m_tilegen[2]->pf_control_0_w(1,0x0003, 0x00ff);
	m_tilegen[2]->pf_control_0_w(2,0x0000, 0x00ff);
	m_tilegen[2]->pf_control_0_w(3,0x0001, 0x00ff); // dimensions

	// scroll registers got written elsewhere, copy them across
	m_tilegen[0]->pf_control_1_w(0,0x0000, 0xffff); // no scroll?
	m_tilegen[0]->pf_control_1_w(1,0x0000, 0xffff); // no scroll?

	m_tilegen[1]->pf_control_1_w(0,m_automat_scroll_regs[3] - 0x010a, 0xffff);
	m_tilegen[1]->pf_control_1_w(1,m_automat_scroll_regs[2], 0xffff);

	m_tilegen[2]->pf_control_1_w(0,m_automat_scroll_regs[1] - 0x0108, 0xffff);
	m_tilegen[2]->pf_control_1_w(1,m_automat_scroll_regs[0], 0xffff);


	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	if (m_pri & 0x04)
		trans = 0x08;
	else
		trans = 0x00;

	if (m_pri & 0x01)
	{
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);

		if (m_pri & 0x02)
			m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, trans, 0x0f);

		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}
	else
	{
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);

		if (m_pri & 0x02)
			m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, trans, 0x0f);

		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}

	if (m_pri & 0x02)
		m_spritegen->draw_sprites_bootleg(bitmap, cliprect, m_buffered_spriteram, 0x08, trans^0x08, 0x0f);
	else
		m_spritegen->draw_sprites_bootleg(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);

	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}

uint32_t dec0_automat_state::screen_update_secretab(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	// layer enables seem different... where are they?

	// the bootleg doesn't write these registers, I think they're hardcoded?, so fake them for compatibility with our implementation..
	m_tilegen[0]->pf_control_0_w(0,0x0003, 0x00ff); // 8x8
	m_tilegen[0]->pf_control_0_w(1,0x0003, 0x00ff);
	m_tilegen[0]->pf_control_0_w(2,0x0000, 0x00ff);
	m_tilegen[0]->pf_control_0_w(3,0x0001, 0x00ff); // dimensions

	m_tilegen[1]->pf_control_0_w(0,0x0082, 0x00ff); // 16x16
	m_tilegen[1]->pf_control_0_w(1,0x0000, 0x00ff);
	m_tilegen[1]->pf_control_0_w(2,0x0000, 0x00ff);
	m_tilegen[1]->pf_control_0_w(3,0x0001, 0x00ff); // dimensions

	m_tilegen[2]->pf_control_0_w(0,0x0082, 0x00ff); // 16x16
	m_tilegen[2]->pf_control_0_w(1,0x0003, 0x00ff);
	m_tilegen[2]->pf_control_0_w(2,0x0000, 0x00ff);
	m_tilegen[2]->pf_control_0_w(3,0x0001, 0x00ff); // dimensions

	// scroll registers got written elsewhere, copy them across
	m_tilegen[0]->pf_control_1_w(0,0x0000, 0xffff); // no scroll?
	m_tilegen[0]->pf_control_1_w(1,0x0000, 0xffff); // no scroll?

	m_tilegen[1]->pf_control_1_w(0,m_automat_scroll_regs[3] - 0x010a, 0xffff);
	m_tilegen[1]->pf_control_1_w(1,m_automat_scroll_regs[2], 0xffff);

	m_tilegen[2]->pf_control_1_w(0,m_automat_scroll_regs[1] - 0x0108, 0xffff);
	m_tilegen[2]->pf_control_1_w(1,m_automat_scroll_regs[0], 0xffff);

	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
	m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);

	m_spritegen->draw_sprites_bootleg(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);

	/* Redraw top 8 pens of top 8 palettes over sprites */
	if (m_pri & 0x80)
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0,0x08,0x08,0x08,0x08); // upper 8 pens of upper 8 priority marked tiles

	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}


/******************************************************************************/

uint32_t dec0_state::screen_update_birdtry(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	/* This game doesn't have the extra playfield chip on the game board, but
	the palette does show through. */
	bitmap.fill(m_palette->pen(768), cliprect);
	m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);
	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}

/******************************************************************************/

uint32_t dec0_state::screen_update_hippodrm(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	if (m_pri & 0x01)
	{
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}
	else
	{
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}

	m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);
	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}

/******************************************************************************/

uint32_t dec0_state::screen_update_slyspy(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);
	m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);

	m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);

	/* Redraw top 8 pens of top 8 palettes over sprites */
	if (m_pri & 0x80)
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0,0x08,0x08,0x08,0x08); // upper 8 pens of upper 8 priority marked tiles

	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}

/******************************************************************************/

uint32_t dec0_state::screen_update_midres(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	int trans;

	bool flip = m_tilegen[0]->get_flip_state();
	m_tilegen[0]->set_flip_screen(flip);
	m_tilegen[1]->set_flip_screen(flip);
	m_tilegen[2]->set_flip_screen(flip);
	m_spritegen->set_flip_screen(flip);

	if (m_pri & 0x04)
		trans = 0x00;
	else trans = 0x08;

	if (m_pri & 0x01)
	{
		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);

		if (m_pri & 0x02)
			m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, trans, 0x0f);

		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}
	else
	{
		m_tilegen[2]->deco_bac06_pf_draw(bitmap,cliprect,TILEMAP_DRAW_OPAQUE, 0x00, 0x00, 0x00, 0x00);

		if (m_pri & 0x02)
			m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, trans, 0x0f);

		m_tilegen[1]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	}

	if (m_pri & 0x02)
		m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x08, trans ^ 0x08, 0x0f);
	else
		m_spritegen->draw_sprites(bitmap, cliprect, m_buffered_spriteram, 0x00, 0x00, 0x0f);

	m_tilegen[0]->deco_bac06_pf_draw(bitmap,cliprect,0, 0x00, 0x00, 0x00, 0x00);
	return 0;
}


WRITE16_MEMBER(dec0_state::priority_w)
{
	COMBINE_DATA(&m_pri);
}

VIDEO_START_MEMBER(dec0_state,dec0_nodma)
{
	save_item(NAME(m_pri));
	m_buffered_spriteram = m_spriteram->live();
	save_pointer(NAME(m_buffered_spriteram), 0x800/2);
}

VIDEO_START_MEMBER(dec0_state,dec0)
{
	save_item(NAME(m_pri));
	m_buffered_spriteram = m_spriteram->buffer();
	save_pointer(NAME(m_buffered_spriteram), 0x800/2);
}

/******************************************************************************/
